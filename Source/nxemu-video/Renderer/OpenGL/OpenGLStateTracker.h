#pragma once
#include "Util\StateTracker.h"
#include <stdint.h>

enum OpenGLDirtyFlag
{
    OpenGLDirtyFlag_ColorMaskCommon,
    OpenGLDirtyFlag_ColorMasks,
    OpenGLDirtyFlag_ColorMask0,
    OpenGLDirtyFlag_ColorMask7 = OpenGLDirtyFlag_ColorMask0 + 7,
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

    void NotifyColorMask(uint8_t index);

private:
    OpenGLStateTracker(void);
    OpenGLStateTracker(const OpenGLStateTracker&);
    OpenGLStateTracker& operator=(const OpenGLStateTracker&);

    void SetupColorMasks(void);
    void SetupRasterizeEnable(void);
    void SetupFramebufferSRGB(void);
    void SetupFragmentClampColor(void);
    CStateTracker & m_StateTracker;
};
