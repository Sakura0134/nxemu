#pragma once
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

    OpenGLRenderer & m_Renderer;
    CMaxwell3D & m_Maxwell3D;
};