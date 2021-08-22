#include "OpenGLRenderer.h"
#include "MaxwellToOpenGL.h"
#include "EmulatorWindow.h"
#include "VideoNotification.h"
#include "Video.h"
#include <glad/glad.h>

#pragma comment(lib, "opengl32.lib")

OpenGLRenderer::OpenGLRenderer(ISwitchSystem & SwitchSystem, CVideo & Video) : 
    m_EmulatorWindow(Video.Window()),
    m_SwitchSystem(SwitchSystem), 
    m_Video(Video),
    m_StateTracker(Video),
    m_TextureCache(*this, Video),
    m_FenceManager(*this, Video),
    m_QueuedCommands(false)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

bool OpenGLRenderer::Init()
{
    OpenGLImage::InitCompatibleViewTable();

    if (!m_OpenGLWindow.Initialize(m_EmulatorWindow.RenderSurface())) 
    {
        return false;
    }
    if (!GLAD_GL_VERSION_4_3) 
    {
        return false;
    }
    if (!m_TextureCache.Init())
    {
        return false;
    }
    glClearColor(0.0f, 0.0f, 0.0, 0.0f);
    return true;
}

void OpenGLRenderer::InvalidateRegion(uint64_t Addr, uint64_t Size)
{
    if (Addr == 0 || Size == 0) 
    {
        return;
    }
    m_TextureCache.WriteMemory(Addr, Size);
}

void OpenGLRenderer::FlushCommands(void)
{
    if (m_QueuedCommands) 
    {
        m_QueuedCommands = false;
        glFlush();
    }
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

void OpenGLRenderer::Draw(bool /*IsIndexed*/, bool /*IsInstanced*/)
{
    SyncViewport();
    SyncRasterizeEnable();
    SyncPolygonModes();
    SyncColorMask();
    g_Notify->BreakPoint(__FILE__, __LINE__);
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
    if (Regs.StencilEnable != 0)
    {
        glEnable(GL_STENCIL_TEST);
    } 
    else 
    {
        glDisable(GL_STENCIL_TEST);
    }

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
    for (uint32_t i = 0; i < CMaxwell3D::NumViewPorts; i++) 
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
