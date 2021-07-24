#pragma once
#include "OpenGLTypes.h"
#include "OpenGLItemPtr.h"
#include "OpenGLResource.h"
#include "Surface.h"
#include <stdint.h>

class OpenGLImage;
class OpenGLImageView;
typedef OpenGLItemPtr<OpenGLImage> OpenGLImagePtr;
typedef OpenGLItemPtr<OpenGLImageView> OpenGLImageViewPtr;

enum ImageViewFlags 
{
    ImageViewFlag_PreemtiveDownload = 1 << 0,
    ImageViewFlag_Slice = 1 << 1,
};

class OpenGLImageView 
{
    friend OpenGLImageViewPtr;

public:
    OpenGLImageView(OpenGLImageViewType Type, SurfacePixelFormat Format, const OpenGLSubresourceRange & Range);

    void Create(OpenGLTexturePtr * NullTextures, uint32_t NumNullTextures, OpenGLImage * Image);

    bool IsFlagSet(ImageViewFlags Flag) const;

    OpenGLImageViewType Type() const { return m_Type; }
    OpenGLImagePtr & Image() { return m_Image; }
    SurfacePixelFormat Format() const { return m_Format; }
    const OpenGLExtent3D & Size() const { return m_Size; }

private:
    OpenGLImageView();
    OpenGLImageView(const OpenGLImageView&);
    OpenGLImageView & operator=(const OpenGLImageView&);

    void SetupView(OpenGLImageViewType ViewType, OpenGLTexturePtr & Texture, const OpenGLSubresourceRange & view_range);
    bool IsRenderTarget() const; 

    static GLenum PixelFormatToInternalFormat(SurfacePixelFormat PixelFormat);
    static GLenum ImageTarget(OpenGLImageViewType Type, int NumSamples = 1);

    OpenGLImagePtr m_Image;
    SurfacePixelFormat m_Format;
    OpenGLImageViewType m_Type;
    OpenGLTexturePtr m_Texture[OpenGLImageViewType_Last];
    OpenGLTexturePtr m_DefaultTexture;
    GLenum m_OpenGLFormat;
    OpenGLSubresourceRange m_Range;
    OpenGLExtent3D m_Size;
    uint32_t m_Flags;
    uint8_t m_XSource, m_YSource, m_ZSource, m_WSource;
    bool m_Created;
    uint32_t m_Ref;
};
