#pragma once
#include "OpenGLImage.h"
#include "OpenGLImageView.h"
#include "GpuTypes.h"
#include <stdint.h>
#include <vector>
#include <unordered_map>

class OpenGLRenderer;
class CVideo;
class CMaxwell3D;

class OpenGLTextureCache
{
    enum 
    {
        PAGE_BITS = 20
    };

    template <typename T>
    struct NoHash 
    {
        size_t operator()(T Value) const 
        {
            return (size_t)(Value);
        }
    };

    typedef std::vector<OpenGLImagePtr> OpenGLImagePtrList;
    typedef std::unordered_map<uint64_t, OpenGLImagePtrList, NoHash<uint64_t>> PageTables;

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
    CVideoMemory & m_VideoMemory;
    OpenGLImageViewPtr m_ColorBuffer[NumRenderTargets];
    PageTables m_PageTable;
};