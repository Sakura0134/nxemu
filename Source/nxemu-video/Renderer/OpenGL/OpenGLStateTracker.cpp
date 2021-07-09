#include "OpenGLStateTracker.h"
#include "Video.h"

OpenGLStateTracker::OpenGLStateTracker(CVideo& Video) : 
    m_StateTracker(Video.Maxwell3D().StateTracker())
{
    m_StateTracker.IncreaseMaxFlag(OpenGLDirtyFlag_Last);
    m_StateTracker.FlagSetAll();

    SetupColorMasks();
    SetupScissors();
    SetupRasterizeEnable();
    SetupFramebufferSRGB();
    SetupFragmentClampColor();
}

void OpenGLStateTracker::NotifyColorMask(uint8_t index) 
{
    m_StateTracker.FlagSet(OpenGLDirtyFlag_ColorMasks);
    m_StateTracker.FlagSet(OpenGLDirtyFlag_ColorMask0 + index);
}

void OpenGLStateTracker::SetupColorMasks(void) 
{
    uint32_t ColorMaskSize = sizeof(CMaxwell3D::Registers::ColorMask[0]) / (sizeof(uint32_t));
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ColorMaskCommon, 1, OpenGLDirtyFlag_ColorMaskCommon);
    for (uint32_t i = 0; i < NumRenderTargets; i++) 
    {
        m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ColorMask + (i * ColorMaskSize), ColorMaskSize, OpenGLDirtyFlag_ColorMask0 + i);
    }
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ColorMask, sizeof(CMaxwell3D::Registers::ColorMask) / (sizeof(uint32_t)), OpenGLDirtyFlag_ColorMasks);
}

void OpenGLStateTracker::SetupScissors(void)
{
    for (uint8_t i = 0; i < CMaxwell3D::NumViewports; i++) 
    {
        uint32_t Offset = CMaxwell3D::Method_ScissorTest + i * (sizeof(CMaxwell3D::Registers::ScissorTest[0]) / (sizeof(uint32_t)));
        m_StateTracker.SetRegisterFlag(Offset, (sizeof(CMaxwell3D::Registers::ScissorTest[0]) / (sizeof(uint32_t))), OpenGLDirtyFlag_Scissor0 + i);
    }
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ScissorTest, (sizeof(CMaxwell3D::Registers::ScissorTest) / (sizeof(uint32_t))), OpenGLDirtyFlag_Scissors);
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
