#include "OpenGLStateTracker.h"
#include "Video.h"

OpenGLStateTracker::OpenGLStateTracker(CVideo& Video) : 
    m_StateTracker(Video.Maxwell3D().StateTracker())
{
    m_StateTracker.IncreaseMaxFlag(OpenGLDirtyFlag_Last);
    m_StateTracker.FlagSetAll();

    SetupDirtyRenderTargets();
    SetupColorMasks();
    SetupViewports();
    SetupScissors();
    SetupVertexArrays();
    SetupVertexFormat();
    SetupShaders();
    SetupPolygonModes();
    SetupDepthTest();
    SetupStencilTest();
    SetupAlphaTest();
    SetupBlend();
    SetupPrimitiveRestart();
    SetupPolygonOffset();
    SetupMultisampleControl();
    SetupRasterizeEnable();
    SetupFramebufferSRGB();
    SetupLogicOp();
    SetupFragmentClampColor();
    SetupPointSize();
    SetupLineWidth();
    SetupClipControl();
    SetupDepthClampEnabled();
    SetupMisc();
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

void OpenGLStateTracker::SetupVertexArrays(void) 
{
    enum 
    {
        VertexArrayItemSize = (sizeof(CMaxwell3D::Registers::VertexArray[0]) / (sizeof(uint32_t))),
        VertexArrayLimitItemSize = (sizeof(CMaxwell3D::Registers::VertexArrayLimit[0]) / (sizeof(uint32_t))),
    };
    for (uint8_t i = 0; i < CMaxwell3D::NumVertexArrays; ++i) 
    {
        uint32_t ArrayOffset = CMaxwell3D::Method_VertexArray + (i * VertexArrayItemSize);
        m_StateTracker.SetRegisterFlag(ArrayOffset, 3, OpenGLDirtyFlag_VertexBuffers);
        m_StateTracker.SetRegisterFlag(ArrayOffset + 3, 1, OpenGLDirtyFlag_VertexInstances);
        m_StateTracker.SetRegisterFlag(ArrayOffset, 3, OpenGLDirtyFlag_VertexBuffer0 + i);
        m_StateTracker.SetRegisterFlag(ArrayOffset + 3, 1, OpenGLDirtyFlag_VertexInstance0 + i);

        uint32_t LimitOffset = CMaxwell3D::Method_VertexArrayLimit + (i * VertexArrayLimitItemSize);
        m_StateTracker.SetRegisterFlag(LimitOffset, VertexArrayLimitItemSize, OpenGLDirtyFlag_VertexBuffers);
        m_StateTracker.SetRegisterFlag(LimitOffset, VertexArrayLimitItemSize, OpenGLDirtyFlag_VertexBuffer0 + i);

        m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_InstancedArrays + i, 1, OpenGLDirtyFlag_VertexInstances);
        m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_InstancedArrays + i, 1, OpenGLDirtyFlag_VertexInstance0 + i);
    }

    m_StateTracker.MemoryWriteFlagSet(OpenGLDirtyFlag_VertexBuffers);
    for (uint8_t i = 0; i < CMaxwell3D::NumVertexArrays; ++i) 
    {
        m_StateTracker.MemoryWriteFlagSet(OpenGLDirtyFlag_VertexBuffer0 + i);
    }
}

void OpenGLStateTracker::SetupVertexFormat(void)
{
    enum 
    {
        VertexAttribFormatItemSize = (sizeof(CMaxwell3D::Registers::VertexAttribFormat[0]) / (sizeof(uint32_t)))
    };

    for (uint8_t i = 0; i < CMaxwell3D::NumVertexAttributes; i++) 
    {
        uint32_t Offset = CMaxwell3D::Method_VertexAttribFormat + (i * VertexAttribFormatItemSize);
        m_StateTracker.SetRegisterFlag(Offset, VertexAttribFormatItemSize, OpenGLDirtyFlag_VertexFormat0 + i);
    }
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_VertexAttribFormat, CMaxwell3D::NumVertexAttributes, OpenGLDirtyFlag_VertexFormats);
}

void OpenGLStateTracker::SetupViewports(void) 
{
    enum 
    {
        ViewPortTransformItemSize = (sizeof(CMaxwell3D::Registers::ViewPortTransform[0]) / (sizeof(uint32_t))),
        ViewPortTransformSize = (sizeof(CMaxwell3D::Registers::ViewPortTransform) / (sizeof(uint32_t))),
        ViewPortsItemSize = (sizeof(CMaxwell3D::Registers::ViewPorts[0]) / (sizeof(uint32_t))),
        ViewPortsSize = (sizeof(CMaxwell3D::Registers::ViewPorts) / (sizeof(uint32_t))),
    };

    for (uint8_t i = 0, n = sizeof(CMaxwell3D::Registers::ViewPorts) / sizeof(CMaxwell3D::Registers::ViewPorts[0]); i < n; i++)
    {
        uint32_t TransfOffset = CMaxwell3D::Method_ViewPortTransform + (i * ViewPortTransformItemSize);
        uint32_t ViewportOffset = CMaxwell3D::Method_ViewPorts + (i * ViewPortsItemSize);

        m_StateTracker.SetRegisterFlag(TransfOffset, ViewPortTransformItemSize, OpenGLDirtyFlag_Viewport0 + i);
        m_StateTracker.SetRegisterFlag(ViewportOffset, ViewPortsItemSize, OpenGLDirtyFlag_Viewport0 + i);
    }
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ViewPortTransform, ViewPortTransformSize, OpenGLDirtyFlag_Viewports);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ViewPorts, ViewPortsSize, OpenGLDirtyFlag_Viewports);

    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ViewportTransformEnabled, 1, OpenGLDirtyFlag_ViewportTransform);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ViewportTransformEnabled, 1, OpenGLDirtyFlag_Viewports);
}

void OpenGLStateTracker::SetupScissors(void)
{
    enum 
    {
        ScissorTestItemSize = (sizeof(CMaxwell3D::Registers::ScissorTest[0]) / (sizeof(uint32_t))),
        ScissorTestSize = (sizeof(CMaxwell3D::Registers::ScissorTest) / (sizeof(uint32_t))),
    };

    for (uint8_t i = 0, n = sizeof(CMaxwell3D::Registers::ScissorTest) / sizeof(CMaxwell3D::Registers::ScissorTest[0]); i < n; i++) 
    {
        uint32_t Offset = CMaxwell3D::Method_ScissorTest + i * ScissorTestItemSize;
        m_StateTracker.SetRegisterFlag(Offset, ScissorTestItemSize, OpenGLDirtyFlag_Scissor0 + i);
    }
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ScissorTest, ScissorTestSize, OpenGLDirtyFlag_Scissors);
}

void OpenGLStateTracker::SetupShaders(void) 
{
    enum 
    {
        ShaderConfigSize = (sizeof(CMaxwell3D::Registers::ShaderConfig) / (sizeof(uint32_t))),
    };

    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ShaderConfig, ShaderConfigSize, OpenGLDirtyFlag_Shaders);
}

void OpenGLStateTracker::SetupPolygonModes(void) 
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonModeFront, 1, OpenGLDirtyFlag_PolygonModeFront);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonModeFront, 1, OpenGLDirtyFlag_PolygonModes);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonModeBack, 1, OpenGLDirtyFlag_PolygonModeBack);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonModeBack, 1, OpenGLDirtyFlag_PolygonModes);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_FillRectangle, 1, OpenGLDirtyFlag_PolygonModes);
}

void OpenGLStateTracker::SetupDepthTest(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_DepthTestEnable, 1, OpenGLDirtyFlag_DepthTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_DepthWriteEnabled, 1, OpenGLDirtyFlag_DepthMask);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_DepthTestFunc, 1, OpenGLDirtyFlag_DepthTest);
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

void OpenGLStateTracker::SetupAlphaTest(void) 
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_AlphaTestRef, 1, OpenGLDirtyFlag_AlphaTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_AlphaTestFunc, 1, OpenGLDirtyFlag_AlphaTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_AlphaTestEnabled, 1, OpenGLDirtyFlag_AlphaTest);
}

void OpenGLStateTracker::SetupBlend(void) 
{
    enum 
    {
        BlendColorSize = (sizeof(CMaxwell3D::Registers::BlendColor) / (sizeof(uint32_t))),
        IndependentBlendItemSize = (sizeof(CMaxwell3D::Registers::IndependentBlend[0]) / (sizeof(uint32_t))),
        IndependentBlendSize = (sizeof(CMaxwell3D::Registers::IndependentBlend) / (sizeof(uint32_t))),
        BlendSize = (sizeof(CMaxwell3D::Registers::Blend) / (sizeof(uint32_t))),
    };

    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_BlendColor, BlendColorSize, OpenGLDirtyFlag_BlendColor);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_IndependentBlendEnable, 1, OpenGLDirtyFlag_BlendIndependentEnabled);

    for (uint8_t i = 0; i < NumRenderTargets; i++) 
    {
        const uint32_t offset = CMaxwell3D::Method_IndependentBlend + i * IndependentBlendItemSize;
        m_StateTracker.SetRegisterFlag(offset, IndependentBlendItemSize, OpenGLDirtyFlag_BlendState0 + i);
        m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_BlendEnable + i, 1, OpenGLDirtyFlag_BlendState0 + i);
    }
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_IndependentBlend, IndependentBlendSize, OpenGLDirtyFlag_BlendStates);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_Blend, BlendSize, OpenGLDirtyFlag_BlendStates);
}

void OpenGLStateTracker::SetupPrimitiveRestart(void)
{
    enum 
    {
        PrimitiveRestartSize = (sizeof(CMaxwell3D::Registers::PrimitiveRestart) / (sizeof(uint32_t))),
    };
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PrimitiveRestart, PrimitiveRestartSize, OpenGLDirtyFlag_PrimitiveRestart);
}

void OpenGLStateTracker::SetupPolygonOffset(void) 
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonOffsetFillEnable, 1, OpenGLDirtyFlag_PolygonOffset);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonOffsetLineEnable, 1, OpenGLDirtyFlag_PolygonOffset);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonOffsetPointEnable, 1, OpenGLDirtyFlag_PolygonOffset);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonOffsetFactor, 1, OpenGLDirtyFlag_PolygonOffset);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonOffsetUnits, 1, OpenGLDirtyFlag_PolygonOffset);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PolygonOffsetClamp, 1, OpenGLDirtyFlag_PolygonOffset);
}

void OpenGLStateTracker::SetupMultisampleControl(void) 
{
    enum 
    {
        MultisampleControlSize = (sizeof(CMaxwell3D::Registers::MultisampleControl) / (sizeof(uint32_t))),
    };
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_MultisampleControl, MultisampleControlSize, OpenGLDirtyFlag_MultisampleControl);
}

void OpenGLStateTracker::SetupRasterizeEnable(void) 
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_RasterizeEnable, 1, OpenGLDirtyFlag_RasterizeEnable);
}

void OpenGLStateTracker::SetupFramebufferSRGB(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_FramebufferSRGB, 1, OpenGLDirtyFlag_FramebufferSRGB);
}

void OpenGLStateTracker::SetupLogicOp(void) 
{
    enum 
    {
        LogicOpSize = (sizeof(CMaxwell3D::Registers::LogicOp) / (sizeof(uint32_t))),
    };
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_LogicOp, LogicOpSize, OpenGLDirtyFlag_LogicOp);
}

void OpenGLStateTracker::SetupFragmentClampColor(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_FragmentColorClamp, 1, OpenGLDirtyFlag_FragmentClampColor);
}

void OpenGLStateTracker::SetupPointSize(void) 
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_VPPointSize, 1, OpenGLDirtyFlag_PointSize);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PointSize, 1, OpenGLDirtyFlag_PointSize);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_PointSpriteEnable, 1, OpenGLDirtyFlag_PointSize);
}

void OpenGLStateTracker::SetupLineWidth(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_LineWidthSmooth, 1, OpenGLDirtyFlag_LineWidth);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_LineWidthAliased, 1, OpenGLDirtyFlag_LineWidth);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_LineSmoothEnable, 1, OpenGLDirtyFlag_LineWidth);
}

void OpenGLStateTracker::SetupClipControl(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ScreenYControl, 1, OpenGLDirtyFlag_ClipControl);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_DepthMode, 1, OpenGLDirtyFlag_ClipControl);
}

void OpenGLStateTracker::SetupDepthClampEnabled(void) 
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_ViewVolumeClipControl, 1, OpenGLDirtyFlag_DepthClampEnabled);
}

void OpenGLStateTracker::SetupMisc(void)
{
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_FrontFace, 1, OpenGLDirtyFlag_FrontFace);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_CullTestEnabled, 1, OpenGLDirtyFlag_CullTest);
    m_StateTracker.SetRegisterFlag(CMaxwell3D::Method_CullFace, 1, OpenGLDirtyFlag_CullTest);
}
