#pragma once
#include "OpenGLImage.h"
#include "OpenGLImageView.h"
#include "OpenGLStagingBuffers.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLTICEntryTable.h"
#include "OpenGLTSCEntryTable.h"
#include "GpuTypes.h"
#include <stdint.h>
#include <vector>
#include <unordered_map>

class OpenGLDevice;
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
    typedef std::vector<OpenGLImageViewPtr> OpenGLImageViews;
    typedef std::unordered_map<uint64_t, OpenGLImagePtr, NoHash<uint64_t>> OpenGLImageImages;
    typedef std::unordered_map<TextureTICEntry, OpenGLImageViewPtr> TextureEntryOpenGLImages;
    typedef std::unordered_map<TextureTSCEntry, OpenGLSamplerPtr> TextureEntryOpenGLSamplers;
    typedef std::unordered_map<uint64_t, OpenGLFramebufferPtr, NoHash<uint64_t>> OpenGLFramebuffers;
    typedef std::vector<OpenGLSamplerPtr> OpenGLSamplers;

public:
    OpenGLTextureCache(OpenGLRenderer & Renderer, CVideo & Video);

    bool Init(const OpenGLDevice & Device);
    void FillGraphicsImageViews(const uint32_t * Indices, uint32_t IndiceSize, OpenGLImageViewPtr * ImageViews, uint32_t ImageViewSize);
    OpenGLSamplerPtr GetGraphicsSampler(uint32_t index);
    void SynchronizeGraphicsDescriptors();
    void UpdateRenderTargets(bool is_clear);
    OpenGLFramebufferPtr GetFramebuffer();
    void WriteMemory(uint64_t CpuAddr, uint64_t Size);
    OpenGLImageViewPtr FindImageView(uint64_t CpuAddress);

private:
    OpenGLTextureCache();
    OpenGLTextureCache(const OpenGLTextureCache &);
    OpenGLTextureCache & operator=(const OpenGLTextureCache &);

    void RefreshContents(OpenGLImage & Image);
    OpenGLImage * GetImage(const OpenGLImage & Info, uint64_t GpuAddr, uint32_t Options = 0);
    OpenGLImageViewPtr FindColorBuffer(size_t Index, bool IsClear);
    OpenGLImageViewPtr FindDepthBuffer(bool IsClear);
    void RegisterImage(OpenGLImagePtr & Image);
    void PrepareImage(OpenGLImagePtr & Image, bool IsModification, bool Invalidate);
    bool IsFullClear(OpenGLImageViewPtr & ImageView);

    OpenGLRenderer & m_Renderer;
    CMaxwell3D & m_Maxwell3D;
    CVideoMemory & m_VideoMemory;
    OpenGLTICEntryTable m_GraphicsImageTable;
    OpenGLTSCEntryTable m_GraphicsSamplerTable;
    OpenGLSamplers m_GraphicsSamplers;
    OpenGLImageViews m_GraphicsImageViews;
    OpenGLImageViewPtr m_ColorBuffer[NumRenderTargets];
    OpenGLImageViewPtr m_DepthBuffer;
    uint8_t m_DrawBuffers[NumRenderTargets];
    uint32_t m_RenderWidth;
    uint32_t m_RenderHeight;
    TextureEntryOpenGLImages m_TextureImageViews;
    TextureEntryOpenGLSamplers m_TextureSamplers;
    PageTables m_PageTable;
    OpenGLImageImages m_Images;
    OpenGLSamplers m_Samplers;
    OpenGLFramebuffers m_Framebuffers;
    OpenGLStagingBuffers m_UploadBuffers;
    OpenGLTexturePtr m_NullImages[OpenGLImageViewType_Last];
    bool m_HasBrokenTextureViewFormats;
};