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
