#include "OpenGLStateTracker.h"
#include "Video.h"

OpenGLStateTracker::OpenGLStateTracker(CVideo& Video) : 
    m_StateTracker(Video.Maxwell3D().StateTracker())
{
    m_StateTracker.IncreaseMaxFlag(OpenGLDirtyFlag_Last);
    m_StateTracker.FlagSetAll();

    SetupDirtyRenderTargets();
    SetupColorMasks();
    SetupScissors();
    SetupRasterizeEnable();
    SetupFramebufferSRGB();
    SetupFragmentClampColor();
}

void OpenGLStateTracker::BindFramebuffer(OpenGLFramebufferPtr Framebuffer)
{
    if (m_Framebuffer == Framebuffer) 
    {
        return;
    }
    m_Framebuffer = Framebuffer;
    if (m_Framebuffer.Get() != nullptr)
    {
        m_Framebuffer->BindFramebuffer(GL_DRAW_FRAMEBUFFER);
    }
    else 
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);    
    }
}

void OpenGLStateTracker::NotifyColorMask(uint8_t index) 
{
    m_StateTracker.FlagSet(OpenGLDirtyFlag_ColorMasks);
    m_StateTracker.FlagSet(OpenGLDirtyFlag_ColorMask0 + index);
}

void OpenGLStateTracker::SetupDirtyRenderTargets(void)
{
    enum
    {
        TicSize = (sizeof(CMaxwell3D::Registers::Tic) / (sizeof(uint32_t))),
        TscSize = (sizeof(CMaxwell3D::Registers::Tsc) / (sizeof(uint32_t))),
        RenderAreaSize = (sizeof(CMaxwell3D::Registers::RenderArea) / (sizeof(uint32_t))),
        RenderTargetItemSize = (sizeof(CMaxwell3D::Registers::RenderTarget[0]) / (sizeof(uint32_t))),
        ZetaSize = (sizeof(CMaxwell3D::Registers::Zeta) / (sizeof(uint32_t))),
    };
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_Tic, TicSize, OpenGLDirtyFlag_Descriptors);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_Tsc, TscSize, OpenGLDirtyFlag_Descriptors);

    uint32_t Num = RenderTargetItemSize * NumRenderTargets;
    for (uint32_t i = 0; i < NumRenderTargets; i++)
    {
        m_StateTracker.SetRegisterFlag((uint32_t)(CMaxwell3D::Method_RenderTarget + i * RenderTargetItemSize), (uint32_t)RenderTargetItemSize, OpenGLDirtyFlag_ColorBuffer0 + i);
    }
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_RenderTarget, Num, OpenGLDirtyFlag_RenderTargets);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_RenderArea, RenderAreaSize, OpenGLDirtyFlag_RenderTargets);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_RTControl, 1, OpenGLDirtyFlag_RenderTargets);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_RTControl, 1, OpenGLDirtyFlag_RenderTargetControl);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ZetaEnable, 1, OpenGLDirtyFlag_ZetaBuffer);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ZetaWidth, 1, OpenGLDirtyFlag_ZetaBuffer);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ZetaHeight, 1, OpenGLDirtyFlag_ZetaBuffer);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_Zeta, ZetaSize, OpenGLDirtyFlag_ZetaBuffer);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ZetaEnable, 1, OpenGLDirtyFlag_RenderTargets);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ZetaWidth, 1, OpenGLDirtyFlag_RenderTargets);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ZetaHeight, 1, OpenGLDirtyFlag_RenderTargets);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_Zeta, ZetaSize, OpenGLDirtyFlag_RenderTargets);
}

void OpenGLStateTracker::SetupColorMasks(void) 
{
    enum 
    {
        ColorMaskItemSize = (sizeof(CMaxwell3D::Registers::ColorMask[0]) / (sizeof(uint32_t))),
        ColorMaskSize = (sizeof(CMaxwell3D::Registers::ColorMask) / (sizeof(uint32_t))),
    };
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ColorMaskCommon, 1, OpenGLDirtyFlag_ColorMaskCommon);
    for (uint32_t i = 0; i < NumRenderTargets; i++) 
    {
        m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ColorMask + (i * ColorMaskItemSize), ColorMaskItemSize, OpenGLDirtyFlag_ColorMask0 + i);
    }
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ColorMask, ColorMaskSize, OpenGLDirtyFlag_ColorMasks);
}

void OpenGLStateTracker::SetupScissors(void)
{
    enum 
    {
        ScissorTestItemSize = (sizeof(CMaxwell3D::Registers::ScissorTest[0]) / (sizeof(uint32_t))),
        ScissorTestSize = (sizeof(CMaxwell3D::Registers::ScissorTest) / (sizeof(uint32_t))),
    };

    for (uint8_t i = 0; i < CMaxwell3D::NumViewPorts; i++) 
    {
        uint32_t Offset = CMaxwell3D::Method_ScissorTest + i * ScissorTestItemSize;
        m_StateTracker.SetRegisterFlag(Offset, ScissorTestItemSize, OpenGLDirtyFlag_Scissor0 + i);
    }
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ScissorTest, ScissorTestSize, OpenGLDirtyFlag_Scissors);
}

void OpenGLStateTracker::SetupStencilTest(void) 
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilEnable, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilFrontFuncFunc, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilFrontFuncRef, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilFrontFuncMask, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilFrontOpFail, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilFrontOpZFail, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilFrontOpZPass, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilFrontMask, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilTwoSideEnable, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilBackFuncFunc, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilBackFuncRef, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilBackFuncMask, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilBackOpFail, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilBackOpZFail, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilBackOpZPass, 1, OpenGLDirtyFlag_StencilTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_StencilBackMask, 1, OpenGLDirtyFlag_StencilTest);
}
void OpenGLStateTracker::SetupRasterizeEnable(void) 
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_RasterizeEnable, 1, OpenGLDirtyFlag_RasterizeEnable);
}

void OpenGLStateTracker::SetupFramebufferSRGB(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_FramebufferSRGB, 1, OpenGLDirtyFlag_FramebufferSRGB);
}

void OpenGLStateTracker::SetupFragmentClampColor(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_FragmentColorClamp, 1, OpenGLDirtyFlag_FragmentClampColor);
}
