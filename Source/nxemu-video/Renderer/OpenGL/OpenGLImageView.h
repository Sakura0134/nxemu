#pragma once
#include "OpenGLItemPtr.h"
#include <stdint.h>

class OpenGLImageView;
typedef OpenGLItemPtr<OpenGLImageView> OpenGLImageViewPtr;

class OpenGLImageView 
{
    friend OpenGLImageViewPtr;

public:
    OpenGLImageView();

private:
    OpenGLImageView(const OpenGLImageView&);
    OpenGLImageView & operator=(const OpenGLImageView&);

    uint32_t m_Ref;
};
