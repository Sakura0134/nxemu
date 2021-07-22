#include "OpenGLImageView.h"
#include "VideoNotification.h"

OpenGLImageView::OpenGLImageView(OpenGLImageViewType Type, SurfacePixelFormat Format, const OpenGLSubresourceRange & /*Range*/) :
    m_Format(Format), 
    m_Type(Type), 
    m_Ref(0)
{
}

void OpenGLImageView::Create(OpenGLTexturePtr * /*NullTextures*/, uint32_t /*NumNullTextures*/, OpenGLImage * /*Image*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

