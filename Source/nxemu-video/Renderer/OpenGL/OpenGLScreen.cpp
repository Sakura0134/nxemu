#include "OpenGLScreen.h"
#include "OpenGLPresentVert.h"
#include "OpenGLPresentFrag.h"
#include "OpenGLStateTracker.h"
#include "OpenGLProgramManager.h"
#include "OpenGLDevice.h"
#include "OpenGLImageView.h"
#include "OpenGLTextureCache.h"
#include "Surface.h"
#include "VideoNotification.h"
#include <glad\glad.h>

struct ScreenRectVertex
{
    ScreenRectVertex(uint32_t x, uint32_t y, GLfloat u, GLfloat v)
    {
        Position[0] = (GLfloat)x;
        Position[1] = (GLfloat)y;
        TexCoord[0] = u;
        TexCoord[1] = v;
    }

    GLfloat Position[2];
    GLfloat TexCoord[2];
};

OpenGLScreen::OpenGLScreen(ISwitchSystem & SwitchSystem, OpenGLStateTracker & StateTracker, OpenGLTextureCache & TextureCache, OpenGLProgramManager & ProgramManager, OpenGLDevice & Device) :
    m_SwitchSystem(SwitchSystem),
    m_StateTracker(StateTracker),
    m_TextureCache(TextureCache),
    m_ProgramManager(ProgramManager),
    m_Device(Device),
    m_VertexBufferAddress(0),
    m_DisplayTexCoords(0.0f, 0.0f, 1.0f, 1.0f),
    m_DisplaySRGB(false),
    m_FramebufferTransformFlags(TransformFlags_Unset),
    m_FramebufferCropRect(0, 0, 0, 0),
    m_GLType(GL_UNSIGNED_INT_8_8_8_8_REV),
    m_GLFormat(GL_RGBA),
    m_PixelFormat(GPUPixelFormat_A8B8G8R8_UNORM),
    m_Width(0),
    m_Height(0)
{
    m_Texture.Reset(new OpenGLTexture);
}

bool OpenGLScreen::Init()
{
    glClearColor(0.0f, 0.0f, 0.0, 0.0f);

    OpenGLShader FragmentShader;
    FragmentShader.Create(OPENGL_PRESENT_FRAG, GL_FRAGMENT_SHADER);
    m_FragmentProgram.Create(true, false, FragmentShader);

    OpenGLShader VertexShader;
    VertexShader.Create(OPENGL_PRESENT_VERT, GL_VERTEX_SHADER);
    m_VertexProgram.Create(true, false, VertexShader);

    m_Pipeline.Create();
    m_Pipeline.UseProgramStages(GL_VERTEX_SHADER_BIT, m_VertexProgram);
    m_Pipeline.UseProgramStages(GL_FRAGMENT_SHADER_BIT, m_FragmentProgram);

    m_VertexBuffer.Create();
    m_VertexBuffer.NamedBufferData(sizeof(ScreenRectVertex) * 4, nullptr, GL_STREAM_DRAW);

    m_PresentSampler.Create();
    m_PresentSampler.SamplerParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    m_Texture->Create(GL_TEXTURE_2D);
    m_Texture->TextureStorage2D(1, GL_RGBA8, 1, 1);
    m_DisplayTexture = m_Texture;

    uint8_t FramebufferData[4] = {0, 0, 0, 0};
    m_Texture->ClearTexImage(0, GL_RGBA, GL_UNSIGNED_BYTE, FramebufferData);
    return true;
}

void OpenGLScreen::Draw(const EmulatorWindow & EmulatorWindow)
{
    enum
    {
        ModelViewMatrixLocation = 0,
        PositionLocation = 0,
        TexCoordLocation = 1,
    };

    GLfloat OrthoMatrix[3 * 2] =
    {
        2.f / EmulatorWindow.Width(), 0.f, 0.f,
        -2.f / EmulatorWindow.Height(), -1.f, 1.f,
    };
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    m_VertexProgram.ProgramUniformMatrix3x2fv(ModelViewMatrixLocation, 1, GL_FALSE, OrthoMatrix);

    float Left = m_DisplayTexCoords.Left();
    float Right = m_DisplayTexCoords.Right();
    if (m_FramebufferTransformFlags != TransformFlags_Unset)
    {
        if (m_FramebufferTransformFlags == TransformFlags_FlipV)
        {
            Left = m_DisplayTexCoords.Right();
            Right = m_DisplayTexCoords.Left();
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }

    if (m_FramebufferCropRect.Top() != 0 || m_FramebufferCropRect.Left() != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    float ScaleU = 1.f, ScaleV = 1.f;
    if (m_FramebufferCropRect.Width() > 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    if (m_FramebufferCropRect.Height() > 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    const WindowScreen & Screen = EmulatorWindow.Screen();
    ScreenRectVertex Vertices[] =
    {
        ScreenRectVertex(Screen.Left(), Screen.Top(), m_DisplayTexCoords.Top() * ScaleV, Left * ScaleV),
        ScreenRectVertex(Screen.Right(), Screen.Top(), m_DisplayTexCoords.Bottom() * ScaleU, Left * ScaleV),
        ScreenRectVertex(Screen.Left(), Screen.Bottom(), m_DisplayTexCoords.Top() * ScaleU, Right * ScaleV),
        ScreenRectVertex(Screen.Right(), Screen.Bottom(), m_DisplayTexCoords.Bottom() * ScaleU, Right * ScaleV),
    };
    m_VertexBuffer.NamedBufferSubData(0, sizeof(Vertices), Vertices);
    m_StateTracker.NotifyDrawScreen();
    m_ProgramManager.BindHostPipeline(m_Pipeline);

    glEnable(GL_CULL_FACE);
    if (m_DisplaySRGB)
    {
        glEnable(GL_FRAMEBUFFER_SRGB);
    }
    else
    {
        glDisable(GL_FRAMEBUFFER_SRGB);
    }
    glDisable(GL_COLOR_LOGIC_OP);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_RASTERIZER_DISCARD);
    glDisable(GL_ALPHA_TEST);
    glDisablei(GL_BLEND, 0);
    glDisablei(GL_SCISSOR_TEST, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    glViewportIndexedf(0, 0.0f, 0.0f, (GLfloat)EmulatorWindow.Width(), (GLfloat)EmulatorWindow.Height());
    glDepthRangeIndexed(0, 0.0, 0.0);

    glEnableVertexAttribArray(PositionLocation);
    glEnableVertexAttribArray(TexCoordLocation);
    glVertexAttribDivisor(PositionLocation, 0);
    glVertexAttribDivisor(TexCoordLocation, 0);
    glVertexAttribFormat(PositionLocation, 2, GL_FLOAT, GL_FALSE, offsetof(ScreenRectVertex, Position));
    glVertexAttribFormat(TexCoordLocation, 2, GL_FLOAT, GL_FALSE, offsetof(ScreenRectVertex, TexCoord));
    glVertexAttribBinding(PositionLocation, 0);
    glVertexAttribBinding(TexCoordLocation, 0);
    if (m_Device.HasVertexBufferUnifiedMemory())
    {
        glBindVertexBuffer(0, 0, 0, sizeof(ScreenRectVertex));
        glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 0, m_VertexBufferAddress, sizeof(Vertices));
    }
    else
    {
        m_VertexBuffer.BindVertexBuffer(0, 0, sizeof(ScreenRectVertex));
    }

    m_DisplayTexture->BindTextureUnit(0);
    m_PresentSampler.BindSampler(0);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_ProgramManager.RestoreGuestPipeline();
}

void OpenGLScreen::PrepareRendertarget(const CFramebuffer & Framebuffer)
{
    if (m_Width != (GLsizei)Framebuffer.Width() || m_Height != (GLsizei)Framebuffer.Height() || m_PixelFormat != Framebuffer.PixelFormat() || m_FramebufferData.empty())
    {
        ConfigureFramebufferTexture(Framebuffer);
    }
    m_FramebufferTransformFlags = Framebuffer.Flags();
    m_FramebufferCropRect = Framebuffer.CropRect();

    uint64_t FramebufferAddress = Framebuffer.Address() + Framebuffer.Offset();
    OpenGLImageViewPtr ImageView = FramebufferAddress != 0 ? m_TextureCache.FindImageView(FramebufferAddress) : nullptr;
    if (ImageView != nullptr)
    {
        if (ImageView->Size().Width() != Framebuffer.Width() || ImageView->Size().Height() != Framebuffer.Height())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        m_DisplayTexture = ImageView->Texture(OpenGLImageViewType_e2D);
        m_DisplaySRGB = IsSurfacePixelFormatSRGB(ImageView->Format());
    } 
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void OpenGLScreen::ConfigureFramebufferTexture(const CFramebuffer & framebuffer)
{
    m_Width = framebuffer.Width();
    m_Height = framebuffer.Height();
    m_PixelFormat = framebuffer.PixelFormat();

    SurfacePixelFormat PixelFormat = SurfacePixelFormatFromGPUPixelFormat(m_PixelFormat);
    uint32_t BytesPerBlock = SurfacePixelFormatBytesPerBlock(PixelFormat);
    m_FramebufferData.resize(m_Width * m_Height * BytesPerBlock);

    GLint InternalFormat;
    switch (m_PixelFormat)
    {
    case GPUPixelFormat_A8B8G8R8_UNORM:
        InternalFormat = GL_RGBA8;
        m_GLFormat = GL_RGBA;
        m_GLType = GL_UNSIGNED_INT_8_8_8_8_REV;
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        InternalFormat = GL_RGBA8;
    }

    m_Texture->Release();
    m_Texture->Create(GL_TEXTURE_2D);
    m_Texture->TextureStorage2D(1, InternalFormat, m_Width, m_Height);
}
