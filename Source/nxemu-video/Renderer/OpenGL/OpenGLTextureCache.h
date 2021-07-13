#pragma once
#include "OpenGLImage.h"
#include "OpenGLImageView.h"
#include "GpuTypes.h"
#include <stdint.h>

class OpenGLRenderer;
class CVideo;
class CMaxwell3D;

class OpenGLTextureCache
{
public:
    OpenGLTextureCache(OpenGLRenderer & Renderer, CVideo & Video);

    bool Init(void);
    void UpdateRenderTargets(bool is_clear);
    void WriteMemory(uint64_t CpuAddr, uint64_t Size);

private:
    OpenGLTextureCache();
    OpenGLTextureCache(const OpenGLTextureCache &);
    OpenGLTextureCache & operator=(const OpenGLTextureCache &);

    OpenGLImage * GetImage(const OpenGLImage & Info, uint64_t GpuAddr, uint32_t Options = 0);
    OpenGLImageViewPtr FindColorBuffer(size_t Index, bool IsClear);

    OpenGLRenderer & m_Renderer;
    CMaxwell3D & m_Maxwell3D;
    OpenGLImageViewPtr m_ColorBuffer[NumRenderTargets];
};