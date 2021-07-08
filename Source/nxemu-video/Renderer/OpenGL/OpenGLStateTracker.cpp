#include "OpenGLStateTracker.h"
#include "Video.h"

OpenGLStateTracker::OpenGLStateTracker(CVideo& Video) : 
    m_StateTracker(Video.Maxwell3D().StateTracker())
{
    m_StateTracker.IncreaseMaxFlag(OpenGLDirtyFlag_Last);
    m_StateTracker.FlagSetAll();

    SetupColorMasks();
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

void OpenGLStateTracker::SetupFramebufferSRGB(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_FramebufferSRGB, 1, OpenGLDirtyFlag_FramebufferSRGB);
}

void OpenGLStateTracker::SetupFragmentClampColor(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_FragmentColorClamp, 1, OpenGLDirtyFlag_FragmentClampColor);
}
