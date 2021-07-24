#pragma once
#include "Surface.h"
#include "OpenGLImageView.h"
#include "OpenGLTypes.h"
#include "OpenGLItemPtr.h"
#include "OpenGLResource.h"
#include "Engine\Maxwell3D.h"
#include <glad/glad.h>
#include <stdint.h>
#include <vector>

class OpenGLStagingBuffer;
class OpenGLRenderer;
class OpenGLImage;
typedef OpenGLItemPtr<OpenGLImage> OpenGLImagePtr;

enum OpenGLImageType
{
    OpenGLImageType_e1D,
    OpenGLImageType_e2D,
    OpenGLImageType_e3D,
    OpenGLImageType_Linear,
    OpenGLImageType_Buffer,
};

enum ImageFlags
{
    ImageFlag_Converted = 1 << 0,
    ImageFlag_CpuModified = 1 << 1,
    ImageFlag_GpuModified = 1 << 2,
    ImageFlag_Tracked = 1 << 3,
    ImageFlag_Registered = 1 << 4,
};

class OpenGLImage
{
    friend OpenGLImagePtr;

    enum
    {
        MAX_MIP_LEVELS = 14
    };

public:
    OpenGLImage();
    OpenGLImage(const OpenGLImage & Image);
    OpenGLImage(const CMaxwell3D::tyRenderTarget& RenderTarget, uint32_t Samples);
    ~OpenGLImage();

    static void InitCompatibleViewTable(void);

    void Create(uint64_t GpuAddr, uint64_t CpuAddr, OpenGLRenderer * Renderer);
    OpenGLImageViewType ImageViewType(void) const;

    bool IsViewCompatible(SurfacePixelFormat Format, bool BrokenViews) const;
    OpenGLBufferImageList UnswizzleImage(CVideoMemory& VideoMemory, uint64_t gpu_addr, uint8_t * Output, size_t OutputSize) const;
    void UploadMemory(OpenGLStagingBuffer & Buffer, uint32_t BufferOffset, const OpenGLBufferImage * Images, size_t NoOfImages);
    OpenGLImageView * ImageView(OpenGLTexturePtr * NullTextures, uint32_t NumNullTextures, uint64_t GPUAddr, bool IsClear);
    uint32_t LayerSize(void) const;
    uint32_t MapSizeBytes(void) const;
    uint32_t GuestSizeBytes(void) const;
    uint32_t UnswizzledSizeBytes(void) const;
    bool IsFlagSet(ImageFlags Flag) const;
    void UpdateFlags(uint32_t Add, uint32_t Remove);
    void Track(void);
    void UploadImageContents(CVideoMemory & VideoMemory, OpenGLStagingBuffer & Buffer, uint32_t BufferOffset);

    OpenGLImageType Type(void) const { return m_Type; }
    const OpenGLExtent3D & Size(void) const { return m_Size; }
    uint32_t NumSamples(void) const { return m_NumSamples; }
    uint64_t CpuAddr(void) const { return m_CpuAddr; }
    uint64_t GpuAddr(void) const { return m_GpuAddr; }
    const OpenGLTexture & Texture() const { return m_Texture; }

private:
    OpenGLImage& operator=(const OpenGLImage&);

    struct LevelInfo
    {
        OpenGLExtent3D Size;
        OpenGLExtent3D Block;
        OpenGLExtent2D TileSize;
        uint32_t BytesPerBlockLog2;
        uint32_t TileWidthSpacing;
    };

    void CopyBufferToImage(const OpenGLBufferImage & Image, uint32_t BufferOffset);
    LevelInfo MakeLevelInfo(void) const;
    uint32_t NumBlocksPerLayer(const OpenGLExtent2D & TileSize) const;
    void SetOpenGLFormat(void);
    bool FindBase(uint64_t Addr, OpenGLSubresourceBase & Subresource) const;

    static uint32_t AdjustTileSize(uint32_t Shift, uint32_t UnitFactor, uint32_t Dimension);
    static OpenGLExtent3D AdjustTileSize(const OpenGLExtent3D & Size, const OpenGLExtent2D & TileSize);
    static OpenGLExtent3D LevelTiles(const LevelInfo & Info, uint32_t Level);
    static OpenGLExtent3D NumLevelBlocks(const LevelInfo & Info, uint32_t Level);
    static OpenGLExtent3D TileShift(const LevelInfo & Info, uint32_t Level);
    static OpenGLExtent2D NumGobs(const LevelInfo & Info, uint32_t Level);
    static uint32_t CalculateLevelSize(const LevelInfo & Info, uint32_t Level);
    static uint32_t NumBlocks(const OpenGLExtent3D & Size, const OpenGLExtent2D & TileSize);
    static bool IsSmallerThanGobSize(const OpenGLExtent3D & NumTiles, const OpenGLExtent2D & Gob, uint32_t BlockDepth);
    static OpenGLExtent2D GobSize(uint32_t BytesPerBlockLog2, uint32_t Height, uint32_t TileWidthSpacing);
    static uint32_t BytesPerBlockLog2(uint32_t BytesPerBlock);
    static uint32_t AlignLayerSize(uint32_t SizeBytes, const OpenGLExtent3D & Size, OpenGLExtent3D Block, uint32_t TileSizeY, uint32_t TileWidthSpacing);
    static uint32_t AdjustMipSize(uint32_t size, uint32_t level);
    static OpenGLExtent3D AdjustMipSize(OpenGLExtent3D size, int32_t level);
    static OpenGLExtent3D AdjustMipBlockSize(OpenGLExtent3D NumTiles, OpenGLExtent3D BlockSize, uint32_t Level);
    static void EnableCompatibleView(SurfacePixelFormat FormatA, SurfacePixelFormat FormatB);
    static void EnableCompatibleRange(SurfacePixelFormat * Formats, uint32_t NoOfFormats);

    static uint64_t m_CompatibleViewTable[SurfacePixelFormat_MaxPixelFormat][2];

    OpenGLImageType m_Type;
    SurfacePixelFormat m_Format;
    OpenGLExtent3D m_Size;
    OpenGLExtent3D m_Block;
    uint32_t m_NumSamples;
    uint32_t m_TileWidthSpacing;
    OpenGLSubresourceExtent m_Resources;
    uint32_t m_LayerStride;
    uint32_t m_Flags;
    uint32_t m_MipLevelOffsets[MAX_MIP_LEVELS];
    std::vector<OpenGLImageViewPtr> m_ImageViews;
    OpenGLRenderer * m_Renderer;
    uint64_t m_GpuAddr;
    uint64_t m_CpuAddr;
    uint64_t m_CpuAddrEnd;
    OpenGLTexture m_Texture;
    GLenum m_GLInternalFormat;
    GLenum m_GLFormat;
    GLenum m_GLType;
    uint32_t m_Ref;
};
