#include "OpenGLWindow.h"
#include <Windows.h>
#include <glad\glad.h>

OpenGLWindow::OpenGLWindow(void) :
    m_hGLRC(NULL)
{
}

bool OpenGLWindow::Initialize(void * RenderSurface) 
{
    typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

    HWND hWnd = (HWND)RenderSurface;
    if (hWnd == nullptr)
    {
        return false;
    }
    HDC hDC = GetDC(hWnd);
    if (hDC == nullptr) 
    {
        return false;
    }
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cAuxBuffers = 1;

    int pfm = ChoosePixelFormat(hDC, &pfd);
    if (pfm == 0)
    {
        pfd.cAuxBuffers = 0;
        pfm = ChoosePixelFormat(hDC, &pfd);
    }
    if (pfm == 0)
    {
        return false;
    }
    if (SetPixelFormat(hDC, pfm, &pfd) == 0)
    {
        return false;
    }

    m_hGLRC = wglCreateContext(hDC);
    if (m_hGLRC == nullptr) 
    {
        return false;
    }
    if (!wglMakeCurrent(hDC, (HGLRC)m_hGLRC)) 
    {
        return false;
    }
    if (!gladLoadGL())
    {
        return false;
    }
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (glGetIntegerv == nullptr && wglCreateContextAttribsARB == nullptr) 
    {
        return false;
    }
    GLint MajorVersion = 0, MinorVersion = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);

    if (MajorVersion < 4 || (MajorVersion == 4 && MinorVersion < 3))
    {
        return false;
    }

    const int attribList[] = 
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB,
        MajorVersion,
        WGL_CONTEXT_MINOR_VERSION_ARB,
        MinorVersion,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
        0,
        0
    };
    HGLRC CoreHrc = wglCreateContextAttribsARB(hDC, 0, attribList);
    if (CoreHrc != nullptr) 
    {
        wglDeleteContext((HGLRC)m_hGLRC);
        wglMakeCurrent(hDC, CoreHrc);
        m_hGLRC = CoreHrc;
    }
    return true;
}

void OpenGLWindow::SwapBuffers() 
{
    ::SwapBuffers(wglGetCurrentDC());
}
