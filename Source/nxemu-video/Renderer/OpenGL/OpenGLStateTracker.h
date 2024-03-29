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

    OpenGLDirtyFlag_VertexFormats,
    OpenGLDirtyFlag_VertexFormat0,
    OpenGLDirtyFlag_VertexFormat31 = OpenGLDirtyFlag_VertexFormat0 + 31,

    OpenGLDirtyFlag_VertexBuffers,
    OpenGLDirtyFlag_VertexBuffer0,
    OpenGLDirtyFlag_VertexBuffer31 = OpenGLDirtyFlag_VertexBuffer0 + 31,

    OpenGLDirtyFlag_VertexInstances,
    OpenGLDirtyFlag_VertexInstance0,
    OpenGLDirtyFlag_VertexInstance31 = OpenGLDirtyFlag_VertexInstance0 + 31,

    OpenGLDirtyFlag_ViewportTransform,
    OpenGLDirtyFlag_Viewports,
    OpenGLDirtyFlag_Viewport0,
    OpenGLDirtyFlag_Viewport15 = OpenGLDirtyFlag_Viewport0 + 15,

    OpenGLDirtyFlag_Scissors,
    OpenGLDirtyFlag_Scissor0,
    OpenGLDirtyFlag_Scissor15 = OpenGLDirtyFlag_Scissor0 + 15,

    OpenGLDirtyFlag_ColorMaskCommon,
    OpenGLDirtyFlag_ColorMasks,
    OpenGLDirtyFlag_ColorMask0,
    OpenGLDirtyFlag_ColorMask7 = OpenGLDirtyFlag_ColorMask0 + 7,

    OpenGLDirtyFlag_BlendColor,
    OpenGLDirtyFlag_BlendIndependentEnabled,
    OpenGLDirtyFlag_BlendStates,
    OpenGLDirtyFlag_BlendState0,
    OpenGLDirtyFlag_BlendState7 = OpenGLDirtyFlag_BlendState0 + 7,

    OpenGLDirtyFlag_Shaders,
    OpenGLDirtyFlag_ClipDistances,

    OpenGLDirtyFlag_PolygonModes,
    OpenGLDirtyFlag_PolygonModeFront,
    OpenGLDirtyFlag_PolygonModeBack,

    OpenGLDirtyFlag_FrontFace,
    OpenGLDirtyFlag_CullTest,
    OpenGLDirtyFlag_DepthMask,
    OpenGLDirtyFlag_DepthTest,
    OpenGLDirtyFlag_StencilTest,
    OpenGLDirtyFlag_AlphaTest,
    OpenGLDirtyFlag_PrimitiveRestart,
    OpenGLDirtyFlag_PolygonOffset,
    OpenGLDirtyFlag_MultisampleControl,
    OpenGLDirtyFlag_RasterizeEnable,
    OpenGLDirtyFlag_FramebufferSRGB,
    OpenGLDirtyFlag_LogicOp,
    OpenGLDirtyFlag_FragmentClampColor,
    OpenGLDirtyFlag_PointSize,
    OpenGLDirtyFlag_LineWidth,
    OpenGLDirtyFlag_ClipControl,
    OpenGLDirtyFlag_DepthClampEnabled,

    OpenGLDirtyFlag_Last
};

class CVideo;

class OpenGLStateTracker 
{
public:
    OpenGLStateTracker(CVideo & Video);

    void BindFramebuffer(OpenGLFramebufferPtr Framebuffer);
    void NotifyDrawScreen();
    void NotifyScreenDrawVertexArray();
    void NotifyPolygonModes();
    void NotifyViewport0();
    void NotifyScissor0();
    void NotifyColorMask(uint8_t index);
    void NotifyBlend0();
    void NotifyFramebuffer();
    void NotifyFrontFace();
    void NotifyCullTest();
    void NotifyDepthTest();
    void NotifyStencilTest();
    void NotifyPolygonOffset();
    void NotifyRasterizeEnable();
    void NotifyFramebufferSRGB();
    void NotifyLogicOp();
    void NotifyClipControl();
    void NotifyAlphaTest();

private:
    OpenGLStateTracker(void);
    OpenGLStateTracker(const OpenGLStateTracker&);
    OpenGLStateTracker& operator=(const OpenGLStateTracker&);

    void SetupDirtyRenderTargets(void);
    void SetupColorMasks(void);
    void SetupVertexArrays(void);
    void SetupVertexFormat(void);
    void SetupViewports(void);
    void SetupScissors(void);
    void SetupShaders(void);
    void SetupPolygonModes(void);
    void SetupDepthTest(void);
    void SetupStencilTest(void);
    void SetupAlphaTest(void);
    void SetupBlend(void);
    void SetupPrimitiveRestart(void);
    void SetupPolygonOffset(void);
    void SetupMultisampleControl(void);
    void SetupRasterizeEnable(void);
    void SetupFramebufferSRGB(void);
    void SetupLogicOp(void);
    void SetupFragmentClampColor(void);
    void SetupPointSize(void);
    void SetupLineWidth(void);
    void SetupClipControl(void);
    void SetupDepthClampEnabled(void);
    void SetupMisc(void);

    CStateTracker & m_StateTracker;
    OpenGLFramebufferPtr m_Framebuffer;
};
