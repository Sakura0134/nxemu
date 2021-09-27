#include "OpenGLRenderer.h"
#include "OpenGLCompiledShader.h"
#include "MaxwellToOpenGL.h"
#include "EmulatorWindow.h"
#include "VideoNotification.h"
#include "Video.h"
#include <Common/Align.h>
#include <glad/glad.h>

#pragma comment(lib, "opengl32.lib")

OpenGLRenderer::OpenGLRenderer(ISwitchSystem & SwitchSystem, CVideo & Video) :
    m_EmulatorWindow(Video.Window()),
    m_SwitchSystem(SwitchSystem),
    m_Video(Video),
    m_StateTracker(Video),
    m_Screen(SwitchSystem, m_StateTracker, m_TextureCache, m_ProgramManager, m_Device),
    m_StreamBuffer(SwitchSystem, Video, m_StateTracker),
    m_TextureCache(*this, Video),
    m_ShaderCache(*this, Video, m_Device),
    m_FenceManager(*this, Video),
    m_QueuedCommands(false),
    m_IsTextureHandlerSizeKnown(true),
    m_LastClipDistanceMask(0)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

bool OpenGLRenderer::Init()
{
    InitOpenGLDecompileShader();
    OpenGLImage::InitCompatibleViewTable();

    if (!m_OpenGLWindow.Initialize(m_EmulatorWindow.RenderSurface()))
    {
        return false;
    }
    if (!GLAD_GL_VERSION_4_3)
    {
        return false;
    }
    if (!m_Device.Init())
    {
        return false;
    }
    if (!m_ProgramManager.Init(m_Device))
    {
        return false;
    }
    if (!m_StreamBuffer.Init(m_Device))
    {
        return false;
    }
    if (!m_TextureCache.Init(m_Device))
    {
        return false;
    }
    if (!m_Screen.Init()) 
    {
        return false;
    }
    return true;
}

void OpenGLRenderer::SwapBuffers(const CFramebuffer & Framebuffer)
{
    m_Screen.PrepareRendertarget(Framebuffer);
    m_StateTracker.BindFramebuffer(0);
    m_Screen.Draw(m_EmulatorWindow);
    TickFrame();

    m_OpenGLWindow.SwapBuffers();
}

void OpenGLRenderer::InvalidateRegion(uint64_t CpuAddr, uint32_t Size)
{
    if (CpuAddr == 0 || Size == 0)
    {
        return;
    }
    m_TextureCache.WriteMemory(CpuAddr, Size);
    m_ShaderCache.InvalidateRegion(CpuAddr, Size);
}

void OpenGLRenderer::FlushCommands(void)
{
    if (m_QueuedCommands)
    {
        m_QueuedCommands = false;
        glFlush();
    }
}

void OpenGLRenderer::SyncGuestHost(void)
{
    m_ShaderCache.SyncGuestHost();
}

void OpenGLRenderer::WaitForIdle(void)
{
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_ELEMENT_ARRAY_BARRIER_BIT |
                    GL_UNIFORM_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT |
                    GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_COMMAND_BARRIER_BIT |
                    GL_PIXEL_BUFFER_BARRIER_BIT | GL_TEXTURE_UPDATE_BARRIER_BIT |
                    GL_BUFFER_UPDATE_BARRIER_BIT | GL_TRANSFORM_FEEDBACK_BARRIER_BIT |
                    GL_SHADER_STORAGE_BARRIER_BIT | GL_QUERY_BUFFER_BARRIER_BIT);
}

void OpenGLRenderer::SignalSyncPoint(uint32_t Value)
{
    m_FenceManager.SignalSyncPoint(Value);
}

void OpenGLRenderer::SignalSemaphore(uint64_t Addr, uint32_t Value)
{
    m_FenceManager.SignalSemaphore(Addr, Value);
}

void OpenGLRenderer::ReleaseFences(void)
{
    m_FenceManager.WaitPendingFences();
}

void OpenGLRenderer::Clear()
{
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    bool UseColor = false, UseDepth = false, UseStencil = false;
    if (Regs.ClearBuffers.R != 0 || Regs.ClearBuffers.G != 0 || Regs.ClearBuffers.B != 0 || Regs.ClearBuffers.A != 0)
    {
        UseColor = true;

        GLuint Index = Regs.ClearBuffers.RT;
        m_StateTracker.NotifyColorMask((uint8_t)Index);
        glColorMaski(Index, Regs.ClearBuffers.R != 0, Regs.ClearBuffers.G != 0, Regs.ClearBuffers.B != 0, Regs.ClearBuffers.A != 0);

        SyncFragmentColorClampState();
        SyncFramebufferSRGB();
    }
    if (Regs.ClearBuffers.Z != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    if (Regs.ClearBuffers.S != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    if (!UseColor && !UseDepth && !UseStencil)
    {
        return;
    }

    SyncRasterizeEnable();
    SyncStencilTestState();

    if (Regs.ClearFlags.Scissor)
    {
        SyncScissorTest();
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    if (Regs.ClearFlags.Viewport != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    m_TextureCache.UpdateRenderTargets(true);
    m_StateTracker.BindFramebuffer(m_TextureCache.GetFramebuffer());

    if (UseColor)
    {
        glClearBufferfv(GL_COLOR, Regs.ClearBuffers.RT, Regs.ClearColor);
    }
    if (UseDepth && UseStencil)
    {
        glClearBufferfi(GL_DEPTH_STENCIL, 0, Regs.ClearDepth, Regs.ClearStencil);
    }
    else if (UseDepth)
    {
        glClearBufferfv(GL_DEPTH, 0, &Regs.ClearDepth);
    }
    else if (UseStencil)
    {
        glClearBufferiv(GL_STENCIL, 0, &Regs.ClearStencil);
    }
    m_QueuedCommands = true;
}

void OpenGLRenderer::Draw(bool IsIndexed, bool IsInstanced)
{
    struct alignas(16) MaxwellUniformData
    {
        GLfloat YDirection;
        PADDING_WORDS(3);
    };

    SyncViewport();
    SyncRasterizeEnable();
    SyncPolygonModes();
    SyncColorMask();
    SyncFragmentColorClampState();
    SyncMultiSampleState();
    SyncDepthTestState();
    SyncDepthClamp();
    SyncStencilTestState();
    SyncBlendState();
    SyncLogicOpState();
    SyncCullMode();
    SyncPrimitiveRestart();
    SyncScissorTest();
    SyncPointState();
    SyncLineState();
    SyncPolygonOffset();
    SyncAlphaTest();
    SyncFramebufferSRGB();

    CMaxwell3D & Maxwell3D = m_Video.Maxwell3D();
    const CMaxwell3D::Registers & Regs = Maxwell3D.Regs();
    uint32_t BufferSize = CalculateVertexArraysSize();
    if (IsIndexed)
    {
        BufferSize = AlignUp(BufferSize, 4) + (Regs.IndexArray.Count * Regs.IndexArray.FormatSizeInBytes());
    }
    BufferSize = AlignUp(BufferSize, 4) + ((sizeof(MaxwellUniformData) + m_Device.GetUniformBufferAlignment()) * CMaxwell3D::MaxShaderStage);
    BufferSize += CMaxwell3D::MaxConstBuffers * (CMaxwell3D::MaxConstBufferSize + m_Device.GetUniformBufferAlignment());
    m_StreamBuffer.Map(BufferSize);

    SetupVertexFormat();
    SetupVertexBuffer();
    SetupVertexInstances();
    GLintptr IndexBufferOffset = IsIndexed ? SetupIndexBuffer() : 0;

    if (!m_Device.UseAssemblyShaders())
    {
        MaxwellUniformData UBO;
        UBO.YDirection = Regs.ScreenYControl.YNegate == 0 ? 1.0f : -1.0f;
        uint64_t Offset = m_StreamBuffer.UploadHostMemory(&UBO, sizeof(UBO), m_Device.GetUniformBufferAlignment());
        m_StreamBuffer.Buffer()->BindBufferRange(GL_UNIFORM_BUFFER, 0, Offset, sizeof(UBO));
    }

    SetupShaders();
    m_StreamBuffer.Unmap();
    m_TextureCache.UpdateRenderTargets(false);
    m_StateTracker.BindFramebuffer(m_TextureCache.GetFramebuffer());
    m_ProgramManager.BindGraphicsPipeline();

    GLenum PrimitiveMode = MaxwellToOpenGL_PrimitiveTopology(Regs.Draw.Topology);
    BeginTransformFeedback(PrimitiveMode);

    GLuint BaseInstance = (GLuint)Regs.VBBaseInstance;
    GLsizei NumInstances = (GLsizei)(IsInstanced ? Maxwell3D.MMEDraw().InstanceCount : 1);
    if (IsIndexed)
    {
        GLint BaseVertex = (GLint)Regs.VBElementBase;
        GLsizei NumVertices = (GLsizei)Regs.IndexArray.Count;
        const GLvoid * BufferOffset = (const GLvoid *)IndexBufferOffset;
        GLenum Format = MaxwellToOpenGL_IndexFormat(Regs.IndexArray.Format);
        if (NumInstances == 1 && BaseInstance == 0 && BaseVertex == 0)
        {
            glDrawElements(PrimitiveMode, NumVertices, Format, BufferOffset);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    EndTransformFeedback();
    m_QueuedCommands = true;
}

bool OpenGLRenderer::IsTextureHandlerSizeKnown() const
{
    return m_IsTextureHandlerSizeKnown;
}

void OpenGLRenderer::TickFrame()
{
    m_QueuedCommands = false;
    m_FenceManager.TickFrame();
}

void OpenGLRenderer::TrackRasterizerMemory(uint64_t CpuAddr, uint64_t Size, bool Track)
{
    enum
    {
        PageBits = 12,
        PagSize = 1ULL << PageBits,
    };
    CGuard Guard(m_PageCS);
    TrackedPageMap::iterator itr = m_TrackedPages.find(CpuAddr);
    if (itr != m_TrackedPages.end())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else
    {
        if (!Track)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        TrackedPage Page;
        Page.Start = CpuAddr >> PageBits;
        Page.Size = (Size + PagSize - 1) >> PageBits;
        m_TrackedPages.insert(TrackedPageMap::value_type(CpuAddr, Page));
        m_SwitchSystem.MarkRasterizerMemory(Page.Start << PageBits, Page.Size << PageBits, true);
    }
}

void OpenGLRenderer::SyncFragmentColorClampState()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_FragmentClampColor))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_FragmentClampColor);

    glClampColor(GL_CLAMP_FRAGMENT_COLOR, m_Video.Maxwell3D().Regs().FragmentColorClamp ? GL_TRUE : GL_FALSE);
}

void OpenGLRenderer::SyncFramebufferSRGB()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_FramebufferSRGB))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_FramebufferSRGB);
    m_Video.Maxwell3D().Regs().FramebufferSRGB != 0 ? glEnable(GL_FRAMEBUFFER_SRGB) : glDisable(GL_FRAMEBUFFER_SRGB);
}

void OpenGLRenderer::SyncRasterizeEnable()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_RasterizeEnable))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_RasterizeEnable);
    m_Video.Maxwell3D().Regs().RasterizeEnable == 0 ? glEnable(GL_RASTERIZER_DISCARD) : glDisable(GL_RASTERIZER_DISCARD);
}

void OpenGLRenderer::SyncStencilTestState()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_StencilTest))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_StencilTest);

    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    OpenGLEnable(GL_STENCIL_TEST, Regs.StencilEnable != 0);
    glStencilFuncSeparate(GL_FRONT, MaxwellToOpenGL_ComparisonOp(Regs.StencilFrontFuncFunc), Regs.StencilFrontFuncRef, Regs.StencilFrontFuncMask);
    glStencilOpSeparate(GL_FRONT, MaxwellToOpenGL_StencilOp(Regs.StencilFrontOpFail), MaxwellToOpenGL_StencilOp(Regs.StencilFrontOpZFail), MaxwellToOpenGL_StencilOp(Regs.StencilFrontOpZPass));
    glStencilMaskSeparate(GL_FRONT, Regs.StencilFrontMask);

    if (Regs.StencilTwoSideEnable != 0)
    {
        glStencilFuncSeparate(GL_BACK, MaxwellToOpenGL_ComparisonOp(Regs.StencilBackFuncFunc), Regs.StencilBackFuncRef, Regs.StencilBackFuncMask);
        glStencilOpSeparate(GL_BACK, MaxwellToOpenGL_StencilOp(Regs.StencilBackOpFail), MaxwellToOpenGL_StencilOp(Regs.StencilBackOpZFail), MaxwellToOpenGL_StencilOp(Regs.StencilBackOpZPass));
        glStencilMaskSeparate(GL_BACK, Regs.StencilBackMask);
    }
    else
    {
        glStencilFuncSeparate(GL_BACK, GL_ALWAYS, 0, 0xFFFFFFFF);
        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilMaskSeparate(GL_BACK, 0xFFFFFFFF);
    }
}

void OpenGLRenderer::SyncScissorTest()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_Scissors))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_Scissors);

    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    for (uint32_t i = 0, n = sizeof(Regs.ScissorTest) / sizeof(Regs.ScissorTest[0]); i < n; i++)
    {
        if (!StateTracker.Flag(OpenGLDirtyFlag_Scissor0 + i))
        {
            continue;
        }
        StateTracker.FlagClear(OpenGLDirtyFlag_Scissor0 + i);

        const CMaxwell3D::tyScissorTest & ScissorTest = Regs.ScissorTest[i];
        if (ScissorTest.Enable)
        {
            glEnablei(GL_SCISSOR_TEST, (GLuint)i);
            glScissorIndexed((GLuint)i, ScissorTest.MinX, ScissorTest.MinY, ScissorTest.MaxX - ScissorTest.MinX, ScissorTest.MaxY - ScissorTest.MinY);
        }
        else
        {
            glDisablei(GL_SCISSOR_TEST, (GLuint)i);
        }
    }
}

void OpenGLRenderer::BindTextures(const OpenGLCompiledShaderPtr * Shaders, uint32_t NoOfShaders)
{
    uint32_t ImageViewIndex = 0;
    uint32_t TextureIndex = 0;
    for (uint32_t ShaderIndex = 0; ShaderIndex < NoOfShaders; ShaderIndex++)
    {
        if (Shaders[ShaderIndex] == nullptr)
        {
            continue;
        }
        const OpenGLDevice::BaseBindings & Base = m_Device.GetBaseBindings((ShaderType)ShaderIndex);
        const OpenGLCompiledShader & Shader = Shaders[ShaderIndex];
        const ShaderSamplerEntryList & Samplers = Shader.Samplers();
        uint32_t BaseTextureIndex = TextureIndex;
        for (uint32_t SamplersIndex = 0, NumSamplers = (uint32_t)Samplers.size(); SamplersIndex < NumSamplers; SamplersIndex++)
        {
            const ShaderSamplerEntry & Sampler = Samplers[SamplersIndex];
            for (uint32_t SamplerIndex = 0, NumSampler = (uint32_t)Sampler.Size(); SamplerIndex < NumSampler; SamplerIndex++)
            {
                const OpenGLImageViewPtr & ImageView = m_ImageViews[ImageViewIndex++];
                OpenGLTexturePtr & Texture = m_Textures[TextureIndex++];
                Texture = ImageView.Get() != nullptr ? ImageView->Texture(ImageViewTypeFromEntry(Sampler)) : OpenGLTexturePtr(nullptr);
            }
        }
        for (uint32_t SamplerIndex = 0, NumSamplers = (uint32_t)Samplers.size(); SamplerIndex < NumSamplers; SamplerIndex++)
        {
            m_Textures[BaseTextureIndex + SamplerIndex]->BindTexture(Base.Sampler);
            m_Samplers[BaseTextureIndex + SamplerIndex]->BindTexture(Base.Sampler);
        }
    }
}

void OpenGLRenderer::BeginTransformFeedback(GLenum /*PrimitiveMode*/)
{
    CMaxwell3D & Maxwell3D = m_Video.Maxwell3D();
    const CMaxwell3D::Registers & Regs = Maxwell3D.Regs();
    if (Regs.TFBEnabled == 0)
    {
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void OpenGLRenderer::EndTransformFeedback()
{
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    if (Regs.TFBEnabled == 0)
    {
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void OpenGLRenderer::SetupVertexFormat()
{
    CMaxwell3D & Maxwell3D = m_Video.Maxwell3D();
    CStateTracker & StateTracker = Maxwell3D.StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_VertexFormats))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_VertexFormats);

    const CMaxwell3D::Registers & Regs = Maxwell3D.Regs();
    for (uint32_t i = 0; i < NUM_SUPPORTED_VERTEX_ATTRIBUTES; i++)
    {
        if (!StateTracker.Flag(OpenGLDirtyFlag_VertexFormat0 + i))
        {
            continue;
        }
        StateTracker.FlagClear(OpenGLDirtyFlag_VertexFormat0 + i);

        const CMaxwell3D::tyVertexAttribute VertexAttrib = Regs.VertexAttribFormat[i];
        if (VertexAttrib.IsConstant())
        {
            glDisableVertexAttribArray(i);
            continue;
        }
        glEnableVertexAttribArray(i);

        if (VertexAttrib.Type == CMaxwell3D::VertexAttributeType_SignedInt || VertexAttrib.Type == CMaxwell3D::VertexAttributeType_UnsignedInt)
        {
            glVertexAttribIFormat(i, VertexAttrib.ComponentCount(), MaxwellToOpenGL_VertexFormat(VertexAttrib), VertexAttrib.Offset);
        }
        else
        {
            glVertexAttribFormat(i, VertexAttrib.ComponentCount(), MaxwellToOpenGL_VertexFormat(VertexAttrib), VertexAttrib.IsNormalized() ? GL_TRUE : GL_FALSE, VertexAttrib.Offset);
        }
        glVertexAttribBinding(i, VertexAttrib.Buffer);
    }
}

void OpenGLRenderer::SetupVertexBuffer()
{
    CMaxwell3D & Maxwell3D = m_Video.Maxwell3D();
    CStateTracker & StateTracker = Maxwell3D.StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_VertexBuffers))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_VertexBuffers);

    const CMaxwell3D::Registers & Regs = Maxwell3D.Regs();
    bool UseUnifiedMemory = m_Device.HasVertexBufferUnifiedMemory();

    for (uint32_t i = 0; i < NUM_SUPPORTED_VERTEX_BINDINGS; i++)
    {
        if (!StateTracker.Flag(OpenGLDirtyFlag_VertexBuffer0 + i))
        {
            continue;
        }
        StateTracker.FlagClear(OpenGLDirtyFlag_VertexBuffer0 + i);

        const CMaxwell3D::tyVertexArray & VertexArray = Regs.VertexArray[i];
        if (!VertexArray.IsEnabled())
        {
            continue;
        }

        uint64_t GpuAddrStart = VertexArray.StartAddress();
        uint64_t GpuAddrEnd = Regs.VertexArrayLimit[i].LimitAddress();
        if (GpuAddrEnd < GpuAddrStart)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }

        uint32_t Size = (uint32_t)(GpuAddrEnd - GpuAddrStart);
        if (Size == 0)
        {
            glBindVertexBuffer(i, 0, 0, VertexArray.Stride);
            if (UseUnifiedMemory)
            {
                glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, i, 0, 0);
            }
            continue;
        }
        uint64_t Offset = m_StreamBuffer.UploadMemory(GpuAddrStart, Size, 4);
        if (UseUnifiedMemory)
        {
            glBindVertexBuffer(i, 0, 0, VertexArray.Stride);
            glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, i, Offset, Size);
        }
        else
        {
            m_StreamBuffer.Buffer()->BindVertexBuffer(i, Offset, VertexArray.Stride);
        }
    }
}

void OpenGLRenderer::SetupVertexInstances()
{
    CMaxwell3D & Maxwell3D = m_Video.Maxwell3D();
    CStateTracker & StateTracker = Maxwell3D.StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_VertexInstances))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_VertexInstances);

    const CMaxwell3D::Registers & Regs = Maxwell3D.Regs();
    for (uint32_t i = 0; i < NUM_SUPPORTED_VERTEX_ATTRIBUTES; i++)
    {
        if (!StateTracker.Flag(OpenGLDirtyFlag_VertexInstance0 + i))
        {
            continue;
        }
        StateTracker.FlagClear(OpenGLDirtyFlag_VertexInstance0 + i);

        bool InstancingEnabled = Regs.InstancedArrays.IsInstancingEnabled(i);
        glVertexBindingDivisor(i, InstancingEnabled ? Regs.VertexArray[i].Divisor : 0);
    }
}

GLintptr OpenGLRenderer::SetupIndexBuffer()
{
    CMaxwell3D & Maxwell3D = m_Video.Maxwell3D();
    const CMaxwell3D::Registers & Regs = Maxwell3D.Regs();
    uint32_t Size = Regs.IndexArray.Count * Regs.IndexArray.FormatSizeInBytes();
    uint64_t Offset = m_StreamBuffer.UploadMemory(Regs.IndexArray.IndexStart(), Size, 4);
    m_StreamBuffer.Buffer()->BindBuffer(GL_ELEMENT_ARRAY_BUFFER);
    return Offset;
}

void OpenGLRenderer::SetupShaders()
{
    m_ImageViewIndices.clear();
    m_Samplers.clear();
    m_TextureCache.SynchronizeGraphicsDescriptors();

    OpenGLCompiledShaderPtr Shaders[CMaxwell3D::MaxShaderStage];
    CMaxwell3D & Maxwell3D = m_Video.Maxwell3D();
    for (uint32_t i = 0; i < CMaxwell3D::MaxShaderProgram; i++)
    {
        CMaxwell3D::ShaderProgram ProgramType = (CMaxwell3D::ShaderProgram)i;

        if (!Maxwell3D.IsShaderConfigEnabled(ProgramType))
        {
            OpenGLProgramPtr NullOpenGLProgramPtr(nullptr);

            switch (ProgramType)
            {
            case CMaxwell3D::ShaderProgram_Geometry:
                m_ProgramManager.UseGeometryShader(NullOpenGLProgramPtr);
                break;
            case CMaxwell3D::ShaderProgram_Fragment:
                m_ProgramManager.UseFragmentShader(NullOpenGLProgramPtr);
                break;
            }
            continue;
        }
        if (ProgramType == CMaxwell3D::ShaderProgram_TesselationControl || ProgramType == CMaxwell3D::ShaderProgram_TesselationEval)
        {
            continue;
        }

        OpenGLCompiledShaderPtr Shader = m_ShaderCache.GetStageProgram(ProgramType);
        OpenGLProgramPtr ShaderProgram(Shader->IsBuilt() ? Shader->Program() : nullptr);
        switch (ProgramType)
        {
        case CMaxwell3D::ShaderProgram_VertexA:
        case CMaxwell3D::ShaderProgram_VertexB:
            m_ProgramManager.UseVertexShader(ShaderProgram);
            break;
        case CMaxwell3D::ShaderProgram_Geometry:
            m_ProgramManager.UseGeometryShader(ShaderProgram);
            break;
        case CMaxwell3D::ShaderProgram_Fragment:
            m_ProgramManager.UseFragmentShader(ShaderProgram);
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }

        uint32_t Stage = ProgramType == CMaxwell3D::ShaderProgram_VertexA ? CMaxwell3D::ShaderProgram_VertexA : ProgramType - 1;
        Shaders[Stage] = Shader;

        SetupDrawConstBuffers(Stage, Shader);
        SetupDrawTextures(Stage, Shader);

        if (ProgramType == CMaxwell3D::ShaderProgram_VertexA)
        {
            i++;
        }
    }
    SyncClipEnabled(0);
    Maxwell3D.StateTracker().FlagClear(OpenGLDirtyFlag_Shaders);
    m_TextureCache.FillGraphicsImageViews(m_ImageViewIndices.data(), (uint32_t)(m_ImageViewIndices.size()), m_ImageViews, (uint32_t)(sizeof(m_ImageViews) / sizeof(m_ImageViews[0])));
    BindTextures(Shaders, sizeof(Shaders) / sizeof(Shaders[0]));
}

void OpenGLRenderer::SetupDrawConstBuffers(uint32_t StageIndex, OpenGLCompiledShaderPtr & Shader)
{
    enum
    {
        NUM_CONST_BUFFERS_PER_STAGE = 18,
        NUM_CONST_BUFFERS_BYTES_PER_STAGE = NUM_CONST_BUFFERS_PER_STAGE * CMaxwell3D::MaxConstBufferSize,
    };
    GLenum PARAMETER_LUT[] =
    {
        GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV,
        GL_TESS_CONTROL_PROGRAM_PARAMETER_BUFFER_NV,
        GL_TESS_EVALUATION_PROGRAM_PARAMETER_BUFFER_NV,
        GL_GEOMETRY_PROGRAM_PARAMETER_BUFFER_NV,
        GL_FRAGMENT_PROGRAM_PARAMETER_BUFFER_NV,
    };
    const OpenGLShaderConstBufferList & ConstBuffers = Shader->ConstBuffers();
    const OpenGLDevice::BaseBindings BaseBindings = m_Device.GetBaseBindings((ShaderType)StageIndex);
    bool UseUnifiedUniforms = Shader->UseUnifiedUniforms();
    uint32_t Binding = m_Device.UseAssemblyShaders() ? 0 : BaseBindings.UniformBuffer;
    for (OpenGLShaderConstBufferList::const_iterator Entry = ConstBuffers.begin(); Entry != ConstBuffers.end(); Entry++)
    {
        uint32_t Index = Entry->GetIndex();
        CMaxwell3D & Maxwell3D = m_Video.Maxwell3D();
        const CMaxwell3D::tyShaderStage & ShaderStage = Maxwell3D.ShaderStage((uint32_t)StageIndex, Index);
        if (ShaderStage.Enabled)
        {
            uint32_t ConstBufferSize = Entry->IsIndirect() ? ShaderStage.Size : Entry->GetSize();
            if (ConstBufferSize > CMaxwell3D::MaxConstBufferSize)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                ConstBufferSize = CMaxwell3D::MaxConstBufferSize;
            }
            uint32_t Size = AlignUp(ConstBufferSize, sizeof(GLfloat[4]));
            uint32_t Alignment = UseUnifiedUniforms ? 4 : m_Device.GetUniformBufferAlignment();
            uint64_t Offset = m_StreamBuffer.UploadMemory(ShaderStage.Address, Size, Alignment);

            if (m_Device.UseAssemblyShaders())
            {
                if (UseUnifiedUniforms)
                {
                    g_Notify->BreakPoint(__FILE__, __LINE__);
                }
                if (Offset != 0)
                {
                    g_Notify->BreakPoint(__FILE__, __LINE__);
                }
                m_StreamBuffer.Buffer()->BindBufferRangeNV(PARAMETER_LUT[StageIndex], Binding, Offset, Size);
            }
            else if (UseUnifiedUniforms)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            else
            {
                m_StreamBuffer.Buffer()->BindBufferRange(GL_UNIFORM_BUFFER, Binding, Offset, Size);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        Binding += 1;
    }
    if (UseUnifiedUniforms)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void OpenGLRenderer::SetupDrawTextures(uint32_t StageIndex, OpenGLCompiledShaderPtr & Shader)
{
    const CMaxwell3D & Maxwell3D = m_Video.Maxwell3D();
    ShaderSamplerEntryList Samplers = Shader->Samplers();
    bool ViaHeaderIndex = Maxwell3D.Regs().SamplerIndex == CMaxwell3D::SamplerIndex_ViaHeaderIndex;
    for (size_t i = 0, n = Samplers.size(); i < n; i++)
    {
        ShaderSamplerEntry & Entry = Samplers[i];
        for (size_t Index = 0, EntrySize = Entry.Size(); Index < EntrySize; Index++)
        {
            if (Entry.IsBindless())
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }

            uint32_t TexCBIndex = Maxwell3D.Regs().TexCBIndex;
            uint64_t Offset = (Entry.Offset() + Index) * sizeof(uint32_t);
            uint32_t ConstBuffer32 = Maxwell3D.AccessConstBuffer32((ShaderType)StageIndex, TexCBIndex, Offset);
            TextureHandle Handle;
            Handle.Value = ConstBuffer32;
            uint32_t ImageID = Handle.TICId;
            uint32_t SamplerID = ViaHeaderIndex ? ImageID : Handle.TSCId;
            OpenGLSamplerPtr Sampler = m_TextureCache.GetGraphicsSampler(SamplerID);
            m_Samplers.push_back(Sampler);
            m_ImageViewIndices.push_back(ImageID);
        }
    }
}

void OpenGLRenderer::SyncViewport()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();

    bool ViewPortDirty = StateTracker.Flag(OpenGLDirtyFlag_Viewports);
    bool ClipControlDirty = StateTracker.Flag(OpenGLDirtyFlag_ClipControl);

    if (ClipControlDirty || StateTracker.Flag(OpenGLDirtyFlag_FrontFace))
    {
        StateTracker.FlagClear(OpenGLDirtyFlag_FrontFace);

        GLenum Mode = MaxwellToOpenGL_FrontFace(Regs.FrontFace);
        if (Regs.ScreenYControl.TriangleRastFlip != 0 && Regs.ViewPortTransform[0].ScaleY < 0.0f)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        glFrontFace(Mode);
    }

    if (ViewPortDirty || StateTracker.Flag(OpenGLDirtyFlag_ClipControl))
    {
        StateTracker.FlagClear(OpenGLDirtyFlag_ClipControl);

        bool FlipY = false;
        if (Regs.ViewPortTransform[0].ScaleY < 0.0f)
        {
            FlipY = !FlipY;
        }
        if (Regs.ScreenYControl.YNegate != 0)
        {
            FlipY = !FlipY;
        }
        glClipControl(FlipY ? GL_UPPER_LEFT : GL_LOWER_LEFT, Regs.DepthMode == CMaxwell3D::DepthMode_ZeroToOne ? GL_ZERO_TO_ONE : GL_NEGATIVE_ONE_TO_ONE);
    }

    if (ViewPortDirty)
    {
        bool ViewportTransformDirty = StateTracker.Flag(OpenGLDirtyFlag_ViewportTransform);
        StateTracker.FlagClear(OpenGLDirtyFlag_Viewports);
        StateTracker.FlagClear(OpenGLDirtyFlag_ViewportTransform);

        for (uint8_t i = 0; i < CMaxwell3D::NumViewPorts; i++)
        {
            if (!ViewportTransformDirty && !StateTracker.Flag(OpenGLDirtyFlag_Viewport0 + i))
            {
                continue;
            }
            StateTracker.FlagClear(OpenGLDirtyFlag_Viewport0 + i);

            const CMaxwell3D::tyViewPortTransform & ViewPortTransform = Regs.ViewPortTransform[i];
            CRectangle<float> Rect = ViewPortTransform.GetRect();
            glViewportIndexedf(i, Rect.Left(), Rect.Bottom(), std::abs(Rect.Width()), std::abs(Rect.Height()));

            GLdouble ReduceZ = Regs.DepthMode == CMaxwell3D::DepthMode_MinusOneToOne;
            GLdouble NearDepth = ViewPortTransform.TranslateZ - ViewPortTransform.ScaleZ * ReduceZ;
            GLdouble FarDepth = ViewPortTransform.TranslateZ + ViewPortTransform.ScaleZ;
            glDepthRangeIndexed(i, NearDepth, FarDepth);

            if (!GLAD_GL_NV_viewport_swizzle)
            {
                continue;
            }
            glViewportSwizzleNV(i, MaxwellToOpenGL_ViewportSwizzle(ViewPortTransform.Swizzle.X), MaxwellToOpenGL_ViewportSwizzle(ViewPortTransform.Swizzle.Y), MaxwellToOpenGL_ViewportSwizzle(ViewPortTransform.Swizzle.Z), MaxwellToOpenGL_ViewportSwizzle(ViewPortTransform.Swizzle.W));
        }
    }
}

void OpenGLRenderer::SyncPolygonModes()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_PolygonModes))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_PolygonModes);

    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    if (Regs.FillRectangle != 0)
    {
        if (!GLAD_GL_NV_fill_rectangle)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            return;
        }
        StateTracker.FlagSet(OpenGLDirtyFlag_PolygonModeFront);
        StateTracker.FlagSet(OpenGLDirtyFlag_PolygonModeBack);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL_RECTANGLE_NV);
        return;
    }

    if (Regs.PolygonModeFront == Regs.PolygonModeBack)
    {
        StateTracker.FlagClear(OpenGLDirtyFlag_PolygonModeFront);
        StateTracker.FlagClear(OpenGLDirtyFlag_PolygonModeBack);
        glPolygonMode(GL_FRONT_AND_BACK, MaxwellToOpenGL_PolygonMode(Regs.PolygonModeFront));
        return;
    }

    if (StateTracker.Flag(OpenGLDirtyFlag_PolygonModeFront))
    {
        StateTracker.FlagClear(OpenGLDirtyFlag_PolygonModeFront);
        glPolygonMode(GL_FRONT, MaxwellToOpenGL_PolygonMode(Regs.PolygonModeFront));
    }

    if (StateTracker.Flag(OpenGLDirtyFlag_PolygonModeBack))
    {
        StateTracker.FlagClear(OpenGLDirtyFlag_PolygonModeBack);
        glPolygonMode(GL_BACK, MaxwellToOpenGL_PolygonMode(Regs.PolygonModeBack));
    }
}

void OpenGLRenderer::SyncColorMask()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_ColorMasks))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_ColorMasks);

    bool Force = StateTracker.Flag(OpenGLDirtyFlag_ColorMaskCommon);
    StateTracker.FlagClear(OpenGLDirtyFlag_ColorMaskCommon);

    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    if (Regs.ColorMaskCommon)
    {
        if (!Force && !StateTracker.Flag(OpenGLDirtyFlag_ColorMask0))
        {
            return;
        }
        StateTracker.FlagClear(OpenGLDirtyFlag_ColorMask0);

        const CMaxwell3D::tyColorMask & Mask = Regs.ColorMask[0];
        glColorMask(Mask.R != 0, Mask.B != 0, Mask.G != 0, Mask.A != 0);
        return;
    }

    for (uint8_t i = 0; i < NumRenderTargets; i++)
    {
        if (!Force && !StateTracker.Flag(OpenGLDirtyFlag_ColorMask0 + i))
        {
            continue;
        }
        StateTracker.FlagClear(OpenGLDirtyFlag_ColorMask0 + i);

        const CMaxwell3D::tyColorMask & Mask = Regs.ColorMask[i];
        glColorMaski(i, Mask.R != 0, Mask.G != 0, Mask.B != 0, Mask.A != 0);
    }
}

void OpenGLRenderer::SyncMultiSampleState()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_MultisampleControl))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_MultisampleControl);

    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    OpenGLEnable(GL_SAMPLE_ALPHA_TO_COVERAGE, Regs.MultisampleControl.AlphaToCoverage);
    OpenGLEnable(GL_SAMPLE_ALPHA_TO_ONE, Regs.MultisampleControl.AlphaToOne);
}

void OpenGLRenderer::SyncDepthTestState()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();

    if (StateTracker.Flag(OpenGLDirtyFlag_DepthMask))
    {
        StateTracker.FlagClear(OpenGLDirtyFlag_DepthMask);
        glDepthMask(Regs.DepthWriteEnabled != 0 ? GL_TRUE : GL_FALSE);
    }

    if (StateTracker.Flag(OpenGLDirtyFlag_DepthTest))
    {
        StateTracker.FlagClear(OpenGLDirtyFlag_DepthTest);
        OpenGLEnable(GL_DEPTH_TEST, Regs.DepthTestEnable != 0);
        if (Regs.DepthTestEnable != 0)
        {
            glDepthFunc(MaxwellToOpenGL_ComparisonOp(Regs.DepthTestFunc));
        }
    }
}

void OpenGLRenderer::SyncDepthClamp()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_DepthClampEnabled))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_DepthClampEnabled);

    OpenGLEnable(GL_DEPTH_CLAMP, m_Video.Maxwell3D().Regs().ViewVolumeClipControl.DepthClampDisabled == 0);
}

void OpenGLRenderer::SyncLogicOpState()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_LogicOp))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_LogicOp);

    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    OpenGLEnable(GL_COLOR_LOGIC_OP, Regs.LogicOp.Enable != 0);
    if (Regs.LogicOp.Enable != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void OpenGLRenderer::SyncCullMode()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();

    if (StateTracker.Flag(OpenGLDirtyFlag_CullTest))
    {
        StateTracker.FlagSet(OpenGLDirtyFlag_CullTest);

        OpenGLEnable(GL_CULL_FACE, Regs.CullTestEnabled != 0);
        if (Regs.CullTestEnabled != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
}

void OpenGLRenderer::SyncPrimitiveRestart()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_PrimitiveRestart))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_PrimitiveRestart);
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();

    OpenGLEnable(GL_PRIMITIVE_RESTART, Regs.PrimitiveRestart.Enabled != 0);
    if (Regs.PrimitiveRestart.Enabled != 0)
    {
        glPrimitiveRestartIndex(Regs.PrimitiveRestart.Index);
    }
}

void OpenGLRenderer::SyncLineState()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_LineWidth))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_LineWidth);

    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    OpenGLEnable(GL_LINE_SMOOTH, Regs.LineSmoothEnable != 0);
    glLineWidth(Regs.LineSmoothEnable ? Regs.LineWidthSmooth : Regs.LineWidthAliased);
}

void OpenGLRenderer::SyncPolygonOffset()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_PolygonOffset))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_PolygonOffset);

    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    OpenGLEnable(GL_POLYGON_OFFSET_FILL, Regs.PolygonOffsetFillEnable != 0);
    OpenGLEnable(GL_POLYGON_OFFSET_LINE, Regs.PolygonOffsetLineEnable != 0);
    OpenGLEnable(GL_POLYGON_OFFSET_POINT, Regs.PolygonOffsetPointEnable != 0);

    if (Regs.PolygonOffsetFillEnable || Regs.PolygonOffsetLineEnable || Regs.PolygonOffsetPointEnable)
    {
        glPolygonOffsetClamp(Regs.PolygonOffsetFactor, Regs.PolygonOffsetUnits / 2.0f, Regs.PolygonOffsetClamp);
    }
}

void OpenGLRenderer::SyncAlphaTest()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_AlphaTest))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_AlphaTest);

    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    OpenGLEnable(GL_ALPHA_TEST, Regs.AlphaTestEnabled != 0);
    if (Regs.AlphaTestEnabled)
    {
        glAlphaFunc(MaxwellToOpenGL_ComparisonOp(Regs.AlphaTestFunc), Regs.AlphaTestRef);
    }
}

void OpenGLRenderer::SyncBlendState()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();

    if (StateTracker.Flag(OpenGLDirtyFlag_BlendColor))
    {
        StateTracker.FlagClear(OpenGLDirtyFlag_BlendColor);
        glBlendColor(Regs.BlendColor.R, Regs.BlendColor.G, Regs.BlendColor.B, Regs.BlendColor.A);
    }

    if (!StateTracker.Flag(OpenGLDirtyFlag_BlendStates))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_BlendStates);

    if (Regs.IndependentBlendEnable != 0)
    {
        OpenGLEnable(GL_BLEND, Regs.Blend.Enable[0] != 0);
        if (Regs.Blend.Enable[0] != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        return;
    }

    bool BlendIndependentEnabled = StateTracker.Flag(OpenGLDirtyFlag_BlendIndependentEnabled);
    StateTracker.FlagClear(OpenGLDirtyFlag_BlendIndependentEnabled);

    for (uint8_t i = 0; i < NumRenderTargets; i++)
    {
        if (!BlendIndependentEnabled && !StateTracker.Flag(OpenGLDirtyFlag_BlendState0 + i))
        {
            continue;
        }
        StateTracker.FlagClear(OpenGLDirtyFlag_BlendState0 + i);

        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void OpenGLRenderer::SyncPointState()
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_PointSize))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_PointSize);
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();

    OpenGLEnable(GL_POINT_SPRITE, Regs.PointSpriteEnable != 0);
    OpenGLEnable(GL_PROGRAM_POINT_SIZE, Regs.VPPointSize.Enable);

    glPointSize(std::max(1.0f, Regs.PointSize));
}

void OpenGLRenderer::SyncClipEnabled(uint32_t ClipMask)
{
    CStateTracker & StateTracker = m_Video.Maxwell3D().StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_ClipDistances) && !StateTracker.Flag(OpenGLDirtyFlag_Shaders))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_ClipDistances);
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();

    ClipMask &= Regs.ClipDistanceEnabled;
    if (ClipMask == m_LastClipDistanceMask)
    {
        return;
    }
    m_LastClipDistanceMask = ClipMask;

    for (uint32_t i = 0; i < CMaxwell3D::NumClipDistances; i++)
    {
        OpenGLEnable((GLenum)(GL_CLIP_DISTANCE0 + i), (ClipMask >> i) & 1);
    }
}

uint32_t OpenGLRenderer::CalculateVertexArraysSize() const
{
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    uint32_t Size = 0;
    for (uint32_t i = 0, n = sizeof(Regs.VertexArray) / sizeof(Regs.VertexArray[0]); i < n; i++)
    {
        if (!Regs.VertexArray[i].IsEnabled())
        {
            continue;
        }

        uint64_t Start = Regs.VertexArray[i].StartAddress();
        uint64_t End = Regs.VertexArrayLimit[i].LimitAddress();
        if (End < Start)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        Size += (uint32_t)(End - Start);
    }
    return Size;
}

void OpenGLRenderer::OpenGLEnable(GLenum Cap, bool Enable)
{
    if (Enable)
    {
        glEnable(Cap);
    }
    else
    {
        glDisable(Cap);
    }
}

OpenGLImageViewType OpenGLRenderer::ImageViewTypeFromEntry(const ShaderSamplerEntry & Entry)
{
    if (Entry.IsBuffer())
    {
        return OpenGLImageViewType_Buffer;
    }
    switch (Entry.Type())
    {
    case ShaderTextureType_2D: return Entry.IsArray() ? OpenGLImageViewType_e2DArray : OpenGLImageViewType_e2D;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return OpenGLImageViewType_e2D;
}
