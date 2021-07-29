#pragma once
#include "OpenGLFramebuffer.h"
#include "Util\StateTracker.h"
#include <stdint.h>

enum OpenGLDirtyFlag
{
    OpenGLDirtyFlag_Descriptors,
    OpenGLDirtyFlag_RenderTargets,
    OpenGLDirtyFlag_RenderTargetControl,
    OpenGLDirtyFlag_ColorBuffer0,
    OpenGLDirtyFlag_ColorBuffer1,
    OpenGLDirtyFlag_ColorBuffer2,
    OpenGLDirtyFlag_ColorBuffer3,
    OpenGLDirtyFlag_ColorBuffer4,
    OpenGLDirtyFlag_ColorBuffer5,
    OpenGLDirtyFlag_ColorBuffer6,
    OpenGLDirtyFlag_ColorBuffer7,
    OpenGLDirtyFlag_ZetaBuffer,

    OpenGLDirtyFlag_Scissors,
    OpenGLDirtyFlag_Scissor0,
    OpenGLDirtyFlag_Scissor15 = OpenGLDirtyFlag_Scissor0 + 15,

    OpenGLDirtyFlag_ColorMaskCommon,
    OpenGLDirtyFlag_ColorMasks,
    OpenGLDirtyFlag_ColorMask0,
    OpenGLDirtyFlag_ColorMask7 = OpenGLDirtyFlag_ColorMask0 + 7,

    OpenGLDirtyFlag_StencilTest,
    OpenGLDirtyFlag_RasterizeEnable,
    OpenGLDirtyFlag_FramebufferSRGB,
    OpenGLDirtyFlag_FragmentClampColor,

    OpenGLDirtyFlag_Last
};

class CVideo;

class OpenGLStateTracker 
{
public:
    OpenGLStateTracker(CVideo & Video);

    void BindFramebuffer(OpenGLFramebufferPtr Framebuffer);
    void NotifyColorMask(uint8_t index);

private:
    OpenGLStateTracker(void);
    OpenGLStateTracker(const OpenGLStateTracker&);
    OpenGLStateTracker& operator=(const OpenGLStateTracker&);

    void SetupDirtyRenderTargets(void);
    void SetupColorMasks(void);
    void SetupScissors(void);
    void SetupStencilTest(void);
    void SetupRasterizeEnable(void);
    void SetupFramebufferSRGB(void);
    void SetupFragmentClampColor(void);

    CStateTracker & m_StateTracker;
    OpenGLFramebufferPtr m_Framebuffer;
};
