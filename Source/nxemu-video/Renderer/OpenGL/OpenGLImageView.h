#pragma once
#include "OpenGLTypes.h"
#include "OpenGLItemPtr.h"
#include "OpenGLResource.h"
#include "Surface.h"
#include <stdint.h>

class OpenGLImage;
class OpenGLImageView;
typedef OpenGLItemPtr<OpenGLImageView> OpenGLImageViewPtr;

class OpenGLImageView 
{
    friend OpenGLImageViewPtr;

public:
    OpenGLImageView(OpenGLImageViewType Type, SurfacePixelFormat Format, const OpenGLSubresourceRange & Range);

    void Create(OpenGLTexturePtr * NullTextures, uint32_t NumNullTextures, OpenGLImage * Image);

    OpenGLImageViewType Type() const { return m_Type; }
    SurfacePixelFormat Format() const { return m_Format; }

private:
    OpenGLImageView();
    OpenGLImageView(const OpenGLImageView&);
    OpenGLImageView & operator=(const OpenGLImageView&);

    SurfacePixelFormat m_Format;
    OpenGLImageViewType m_Type;
    uint32_t m_Ref;
};
