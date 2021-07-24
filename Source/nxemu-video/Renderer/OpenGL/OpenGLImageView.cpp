#include "OpenGLImageView.h"
#include "OpenGLImage.h"
#include "Surface.h"
#include "VideoNotification.h"
#include <algorithm>

enum { RENDER_TARGET_SWIZZLE = 0xFF };

OpenGLImageView::OpenGLImageView(OpenGLImageViewType Type, SurfacePixelFormat Format, const OpenGLSubresourceRange & Range) :
    m_Image(nullptr),
    m_Format(Format), 
    m_Type(Type), 
    m_Range(Range),
    m_Size(),
    m_OpenGLFormat(GL_NONE), 
    m_Flags(0),
    m_XSource(RENDER_TARGET_SWIZZLE),
    m_YSource(RENDER_TARGET_SWIZZLE),
    m_ZSource(RENDER_TARGET_SWIZZLE),
    m_WSource(RENDER_TARGET_SWIZZLE),
    m_Created(false),
    m_Ref(0)
{
}

void OpenGLImageView::Create(OpenGLTexturePtr * NullTextures, uint32_t NumNullTextures, OpenGLImage * Image)
{
    if (m_Created)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_Created = true;
    m_Image = Image;
    m_Size.Width(std::max(Image->Size().Width() >> m_Range.Base().Level(), 1u));
    m_Size.Height(std::max(Image->Size().Height() >> m_Range.Base().Level(), 1u));
    m_Size.Depth(std::max(Image->Size().Depth() >> m_Range.Base().Level(), 1u));

    if (NumNullTextures != (sizeof(m_Texture) / sizeof(m_Texture[0]))) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    for (uint32_t i = 0; i < NumNullTextures; i++)
    {
        m_Texture[i] = NullTextures[i];
    }
    if (!Image->IsViewCompatible(m_Format, false))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    if (Image->Type() == OpenGLImageType_Linear) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    if (Image->Type() == OpenGLImageType_e3D && m_Type != OpenGLImageViewType_e3D)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    m_OpenGLFormat = Image->IsFlagSet(ImageFlag_Converted) ? (IsSurfacePixelFormatSRGB(m_Format) ? GL_SRGB8_ALPHA8 : GL_RGBA8) : PixelFormatToInternalFormat(m_Format);
    OpenGLSubresourceRange FlattenRange = m_Range;
    OpenGLTexturePtr Texture;

    switch (m_Type) 
    {
    case OpenGLImageViewType_e2D:
        if ((m_Flags & ImageViewFlag_Slice) != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }
        Texture.Reset(new OpenGLTexture);
        Texture->Generate();
        SetupView(OpenGLImageViewType_e2D, Texture, FlattenRange);
        Texture.Reset(new OpenGLTexture);
        Texture->Generate();
        SetupView(OpenGLImageViewType_e2DArray, Texture, m_Range);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_DefaultTexture = m_Texture[m_Type];
}

bool OpenGLImageView::IsFlagSet(ImageViewFlags Flag) const 
{
    return (m_Flags & Flag) != 0;
}

void OpenGLImageView::SetupView(OpenGLImageViewType ViewType, OpenGLTexturePtr & Texture, const OpenGLSubresourceRange & view_range) 
{
    if (m_Type == OpenGLImageViewType_Buffer) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else
    {
        GLenum target = ImageTarget(ViewType, m_Image->NumSamples());
        Texture->TextureView(target, m_Image->Texture(), m_OpenGLFormat, view_range.Base().Level(), view_range.Extent().Levels(), view_range.Base().Layer(), view_range.Extent().Layers());
        if (!IsRenderTarget()) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    m_Texture[ViewType] = Texture;
}

bool OpenGLImageView::IsRenderTarget() const 
{
    return m_XSource == RENDER_TARGET_SWIZZLE && m_YSource == RENDER_TARGET_SWIZZLE &&
           m_ZSource == RENDER_TARGET_SWIZZLE && m_WSource == RENDER_TARGET_SWIZZLE;
}

GLenum OpenGLImageView::PixelFormatToInternalFormat(SurfacePixelFormat PixelFormat)
{
    switch (PixelFormat) 
    {
    case SurfacePixelFormat_A8B8G8R8_UNORM: return GL_RGBA8;
    case SurfacePixelFormat_A8B8G8R8_SNORM: return GL_RGBA8_SNORM;
    case SurfacePixelFormat_A8B8G8R8_SINT: return GL_RGBA8I;
    case SurfacePixelFormat_A8B8G8R8_UINT: return GL_RGBA8UI;
    case SurfacePixelFormat_R5G6B5_UNORM: return GL_RGB565;
    case SurfacePixelFormat_B5G6R5_UNORM: return GL_RGB565;
    case SurfacePixelFormat_A1R5G5B5_UNORM: return GL_RGB5_A1;
    case SurfacePixelFormat_A2B10G10R10_UNORM: return GL_RGB10_A2;
    case SurfacePixelFormat_A2B10G10R10_UINT: return GL_RGB10_A2UI;
    case SurfacePixelFormat_A1B5G5R5_UNORM: return GL_RGB5_A1;
    case SurfacePixelFormat_R8_UNORM: return GL_R8;
    case SurfacePixelFormat_R8_SNORM: return GL_R8_SNORM;
    case SurfacePixelFormat_R8_SINT: return GL_R8I;
    case SurfacePixelFormat_R8_UINT: return GL_R8UI;
    case SurfacePixelFormat_R16G16B16A16_FLOAT: return GL_RGBA16F;
    case SurfacePixelFormat_R16G16B16A16_UNORM: return GL_RGBA16;
    case SurfacePixelFormat_R16G16B16A16_SNORM: return GL_RGBA16_SNORM;
    case SurfacePixelFormat_R16G16B16A16_SINT: return GL_RGBA16I;
    case SurfacePixelFormat_R16G16B16A16_UINT: return GL_RGBA16UI;
    case SurfacePixelFormat_B10G11R11_FLOAT: return GL_R11F_G11F_B10F;
    case SurfacePixelFormat_R32G32B32A32_UINT: return GL_RGBA32UI;
    case SurfacePixelFormat_BC1_RGBA_UNORM: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    case SurfacePixelFormat_BC2_UNORM: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    case SurfacePixelFormat_BC3_UNORM: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    case SurfacePixelFormat_BC4_UNORM: return GL_COMPRESSED_RED_RGTC1;
    case SurfacePixelFormat_BC4_SNORM: return GL_COMPRESSED_SIGNED_RED_RGTC1;
    case SurfacePixelFormat_BC5_UNORM: return GL_COMPRESSED_RG_RGTC2;
    case SurfacePixelFormat_BC5_SNORM: return GL_COMPRESSED_SIGNED_RG_RGTC2;
    case SurfacePixelFormat_BC7_UNORM: return GL_COMPRESSED_RGBA_BPTC_UNORM;
    case SurfacePixelFormat_BC6H_UFLOAT: return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
    case SurfacePixelFormat_BC6H_SFLOAT: return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
    case SurfacePixelFormat_ASTC_2D_4X4_UNORM: return GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
    case SurfacePixelFormat_B8G8R8A8_UNORM: return GL_RGBA8;
    case SurfacePixelFormat_R32G32B32A32_FLOAT: return GL_RGBA32F;
    case SurfacePixelFormat_R32G32B32A32_SINT: return GL_RGBA32I;
    case SurfacePixelFormat_R32G32_FLOAT: return GL_RG32F;
    case SurfacePixelFormat_R32G32_SINT: return GL_RG32I;
    case SurfacePixelFormat_R32_FLOAT: return GL_R32F;
    case SurfacePixelFormat_R16_FLOAT: return GL_R16F;
    case SurfacePixelFormat_R16_UNORM: return GL_R16;
    case SurfacePixelFormat_R16_SNORM: return GL_R16_SNORM;
    case SurfacePixelFormat_R16_UINT: return GL_R16UI;
    case SurfacePixelFormat_R16_SINT: return GL_R16I;
    case SurfacePixelFormat_R16G16_UNORM: return GL_RG16;
    case SurfacePixelFormat_R16G16_FLOAT: return GL_RG16F;
    case SurfacePixelFormat_R16G16_UINT: return GL_RG16UI;
    case SurfacePixelFormat_R16G16_SINT: return GL_RG16I;
    case SurfacePixelFormat_R16G16_SNORM: return GL_RG16_SNORM;
    case SurfacePixelFormat_R32G32B32_FLOAT: return GL_RGB32F;
    case SurfacePixelFormat_A8B8G8R8_SRGB: return GL_SRGB8_ALPHA8;
    case SurfacePixelFormat_R8G8_UNORM: return GL_RG8;
    case SurfacePixelFormat_R8G8_SNORM: return GL_RG8_SNORM;
    case SurfacePixelFormat_R8G8_SINT: return GL_RG8I;
    case SurfacePixelFormat_R8G8_UINT: return GL_RG8UI;
    case SurfacePixelFormat_R32G32_UINT: return GL_RG32UI;
    case SurfacePixelFormat_R16G16B16X16_FLOAT: return GL_RGB16F;
    case SurfacePixelFormat_R32_UINT: return GL_R32UI;
    case SurfacePixelFormat_R32_SINT: return GL_R32I;
    case SurfacePixelFormat_ASTC_2D_8X8_UNORM: return GL_COMPRESSED_RGBA_ASTC_8x8_KHR;
    case SurfacePixelFormat_ASTC_2D_8X5_UNORM: return GL_COMPRESSED_RGBA_ASTC_8x5_KHR;
    case SurfacePixelFormat_ASTC_2D_5X4_UNORM: return GL_COMPRESSED_RGBA_ASTC_5x4_KHR;
    case SurfacePixelFormat_B8G8R8A8_SRGB: return GL_SRGB8_ALPHA8;
    case SurfacePixelFormat_BC1_RGBA_SRGB: return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
    case SurfacePixelFormat_BC2_SRGB: return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
    case SurfacePixelFormat_BC3_SRGB: return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
    case SurfacePixelFormat_BC7_SRGB: return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
    case SurfacePixelFormat_A4B4G4R4_UNORM: return GL_RGBA4;
    case SurfacePixelFormat_ASTC_2D_4X4_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR;
    case SurfacePixelFormat_ASTC_2D_8X8_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR;
    case SurfacePixelFormat_ASTC_2D_8X5_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR;
    case SurfacePixelFormat_ASTC_2D_5X4_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR;
    case SurfacePixelFormat_ASTC_2D_5X5_UNORM: return GL_COMPRESSED_RGBA_ASTC_5x5_KHR;
    case SurfacePixelFormat_ASTC_2D_5X5_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR;
    case SurfacePixelFormat_ASTC_2D_10X8_UNORM: return GL_COMPRESSED_RGBA_ASTC_10x8_KHR;
    case SurfacePixelFormat_ASTC_2D_10X8_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR;
    case SurfacePixelFormat_ASTC_2D_6X6_UNORM: return GL_COMPRESSED_RGBA_ASTC_6x6_KHR;
    case SurfacePixelFormat_ASTC_2D_6X6_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR;
    case SurfacePixelFormat_ASTC_2D_10X10_UNORM: return GL_COMPRESSED_RGBA_ASTC_10x10_KHR;
    case SurfacePixelFormat_ASTC_2D_10X10_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR;
    case SurfacePixelFormat_ASTC_2D_12X12_UNORM: return GL_COMPRESSED_RGBA_ASTC_12x12_KHR;
    case SurfacePixelFormat_ASTC_2D_12X12_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR;
    case SurfacePixelFormat_ASTC_2D_8X6_UNORM: return GL_COMPRESSED_RGBA_ASTC_8x6_KHR;
    case SurfacePixelFormat_ASTC_2D_8X6_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR;
    case SurfacePixelFormat_ASTC_2D_6X5_UNORM: return GL_COMPRESSED_RGBA_ASTC_6x5_KHR;
    case SurfacePixelFormat_ASTC_2D_6X5_SRGB: return GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR;
    case SurfacePixelFormat_E5B9G9R9_FLOAT: return GL_RGB9_E5;
    case SurfacePixelFormat_D32_FLOAT: return GL_DEPTH_COMPONENT32F;
    case SurfacePixelFormat_D16_UNORM: return GL_DEPTH_COMPONENT16;
    case SurfacePixelFormat_D24_UNORM_S8_UINT: return GL_DEPTH24_STENCIL8;
    case SurfacePixelFormat_S8_UINT_D24_UNORM: return GL_DEPTH24_STENCIL8;
    case SurfacePixelFormat_D32_FLOAT_S8_UINT: return GL_DEPTH32F_STENCIL8;
    }
    g_Notify->BreakPoint(__FILE__,__LINE__);
    return GL_RGBA8;
}

GLenum OpenGLImageView::ImageTarget(OpenGLImageViewType Type, int NumSamples) 
{
    bool MultiSampled = NumSamples > 1;

    switch (Type)
    {
    case OpenGLImageViewType_e2D: return MultiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    case OpenGLImageViewType_e2DArray: return MultiSampled ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_ARRAY;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return GL_NONE;
}
