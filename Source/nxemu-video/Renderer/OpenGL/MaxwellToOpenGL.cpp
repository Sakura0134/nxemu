#include "MaxwellToOpenGL.h"
#include "VideoNotification.h"

GLenum MaxwellToOpenGL_ComparisonOp(CMaxwell3D::ComparisonOp Comparison)
{
    switch (Comparison) 
    {
    case CMaxwell3D::ComparisonOp_Never:
    case CMaxwell3D::ComparisonOp_NeverOld:
        return GL_NEVER;
    case CMaxwell3D::ComparisonOp_Less:
    case CMaxwell3D::ComparisonOp_LessOld:
        return GL_LESS;
    case CMaxwell3D::ComparisonOp_Equal:
    case CMaxwell3D::ComparisonOp_EqualOld:
        return GL_EQUAL;
    case CMaxwell3D::ComparisonOp_LessEqual:
    case CMaxwell3D::ComparisonOp_LessEqualOld:
        return GL_LEQUAL;
    case CMaxwell3D::ComparisonOp_Greater:
    case CMaxwell3D::ComparisonOp_GreaterOld:
        return GL_GREATER;
    case CMaxwell3D::ComparisonOp_NotEqual:
    case CMaxwell3D::ComparisonOp_NotEqualOld:
        return GL_NOTEQUAL;
    case CMaxwell3D::ComparisonOp_GreaterEqual:
    case CMaxwell3D::ComparisonOp_GreaterEqualOld:
        return GL_GEQUAL;
    case CMaxwell3D::ComparisonOp_Always:
    case CMaxwell3D::ComparisonOp_AlwaysOld:
        return GL_ALWAYS;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return GL_ALWAYS;
}

GLenum MaxwellToOpenGL_FrontFace(CMaxwell3D::FrontFace FrontFace) 
{
    switch (FrontFace)
    {
    case CMaxwell3D::FrontFace_ClockWise: return GL_CW;
    case CMaxwell3D::FrontFace_CounterClockWise: return GL_CCW;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return GL_CCW;
}

GLenum MaxwellToOpenGL_PolygonMode(CMaxwell3D::PolygonMode PolygonMode) 
{
    switch (PolygonMode)
    {
    case CMaxwell3D::PolygonMode_Point: return GL_POINT;
    case CMaxwell3D::PolygonMode_Line: return GL_LINE;
    case CMaxwell3D::PolygonMode_Fill: return GL_FILL;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return GL_FILL;
}

GLenum MaxwellToOpenGL_StencilOp(CMaxwell3D::StencilOp Stencil) 
{
    switch (Stencil) 
    {
    case CMaxwell3D::StencilOp_Keep:
    case CMaxwell3D::StencilOp_KeepOGL:
        return GL_KEEP;
    case CMaxwell3D::StencilOp_Zero:
    case CMaxwell3D::StencilOp_ZeroOGL:
        return GL_ZERO;
    case CMaxwell3D::StencilOp_Replace:
    case CMaxwell3D::StencilOp_ReplaceOGL:
        return GL_REPLACE;
    case CMaxwell3D::StencilOp_Incr:
    case CMaxwell3D::StencilOp_IncrOGL:
        return GL_INCR;
    case CMaxwell3D::StencilOp_Decr:
    case CMaxwell3D::StencilOp_DecrOGL:
        return GL_DECR;
    case CMaxwell3D::StencilOp_Invert:
    case CMaxwell3D::StencilOp_InvertOGL:
        return GL_INVERT;
    case CMaxwell3D::StencilOp_IncrWrap:
    case CMaxwell3D::StencilOp_IncrWrapOGL:
        return GL_INCR_WRAP;
    case CMaxwell3D::StencilOp_DecrWrap:
    case CMaxwell3D::StencilOp_DecrWrapOGL:
        return GL_DECR_WRAP;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return GL_KEEP;
}
GLenum MaxwellToOpenGL_VertexFormat(CMaxwell3D::tyVertexAttribute Attrib) 
{
    switch (Attrib.Type) 
    {
    case CMaxwell3D::VertexAttributeType_UnsignedNorm:
    case CMaxwell3D::VertexAttributeType_UnsignedScaled:
    case CMaxwell3D::VertexAttributeType_UnsignedInt:
        switch (Attrib.Size)
        {
        case CMaxwell3D::VertexAttributeSize_8:
        case CMaxwell3D::VertexAttributeSize_8_8:
        case CMaxwell3D::VertexAttributeSize_8_8_8:
        case CMaxwell3D::VertexAttributeSize_8_8_8_8:
            return GL_UNSIGNED_BYTE;
        case CMaxwell3D::VertexAttributeSize_16:
        case CMaxwell3D::VertexAttributeSize_16_16:
        case CMaxwell3D::VertexAttributeSize_16_16_16:
        case CMaxwell3D::VertexAttributeSize_16_16_16_16:
            return GL_UNSIGNED_SHORT;
        case CMaxwell3D::VertexAttributeSize_32:
        case CMaxwell3D::VertexAttributeSize_32_32:
        case CMaxwell3D::VertexAttributeSize_32_32_32:
        case CMaxwell3D::VertexAttributeSize_32_32_32_32:
            return GL_UNSIGNED_INT;
        case CMaxwell3D::VertexAttributeSize_10_10_10_2:
            return GL_UNSIGNED_INT_2_10_10_10_REV;
        default:
            break;
        }
        break;
    case CMaxwell3D::VertexAttributeType_SignedNorm:
    case CMaxwell3D::VertexAttributeType_SignedScaled:
    case CMaxwell3D::VertexAttributeType_SignedInt:
        switch (Attrib.Size)
        {
        case CMaxwell3D::VertexAttributeSize_8:
        case CMaxwell3D::VertexAttributeSize_8_8:
        case CMaxwell3D::VertexAttributeSize_8_8_8:
        case CMaxwell3D::VertexAttributeSize_8_8_8_8:
            return GL_BYTE;
        case CMaxwell3D::VertexAttributeSize_16:
        case CMaxwell3D::VertexAttributeSize_16_16:
        case CMaxwell3D::VertexAttributeSize_16_16_16:
        case CMaxwell3D::VertexAttributeSize_16_16_16_16:
            return GL_SHORT;
        case CMaxwell3D::VertexAttributeSize_32:
        case CMaxwell3D::VertexAttributeSize_32_32:
        case CMaxwell3D::VertexAttributeSize_32_32_32:
        case CMaxwell3D::VertexAttributeSize_32_32_32_32:
            return GL_INT;
        case CMaxwell3D::VertexAttributeSize_10_10_10_2:
            return GL_INT_2_10_10_10_REV;
        default:
            break;
        }
        break;
    case CMaxwell3D::VertexAttributeType_Float:
        switch (Attrib.Size) 
        {
        case CMaxwell3D::VertexAttributeSize_16:
        case CMaxwell3D::VertexAttributeSize_16_16:
        case CMaxwell3D::VertexAttributeSize_16_16_16:
        case CMaxwell3D::VertexAttributeSize_16_16_16_16:
            return GL_HALF_FLOAT;
        case CMaxwell3D::VertexAttributeSize_32:
        case CMaxwell3D::VertexAttributeSize_32_32:
        case CMaxwell3D::VertexAttributeSize_32_32_32:
        case CMaxwell3D::VertexAttributeSize_32_32_32_32:
            return GL_FLOAT;
        default:
            break;
        }
        break;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return GL_ZERO;
}

GLenum MaxwellToOpenGL_ViewportSwizzle(CMaxwell3D::ViewportSwizzle swizzle) 
{
    return GL_VIEWPORT_SWIZZLE_POSITIVE_X_NV + (GLenum)swizzle;
}
