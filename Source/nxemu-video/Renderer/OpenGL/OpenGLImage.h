#pragma once
#include "Surface.h"
#include "OpenGLTypes.h"
#include "OpenGLItemPtr.h"
#include "Engine\Maxwell3D.h"
#include <stdint.h>

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

class OpenGLImage
{
    friend OpenGLImagePtr;

public:
    OpenGLImage();
    OpenGLImage(const OpenGLImage & Image);
    OpenGLImage(const CMaxwell3D::tyRenderTarget& RenderTarget, uint32_t Samples);
    ~OpenGLImage();

    uint32_t LayerSize(void) const;
    uint32_t GuestSizeBytes(void) const;

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

    LevelInfo MakeLevelInfo(void) const;

    static uint32_t AdjustTileSize(uint32_t Shift, uint32_t UnitFactor, uint32_t Dimension);
    static OpenGLExtent3D LevelTiles(const LevelInfo & Info, uint32_t Level);
    static OpenGLExtent3D NumLevelBlocks(const LevelInfo & Info, uint32_t Level);
    static OpenGLExtent3D TileShift(const LevelInfo & Info, uint32_t Level);
    static OpenGLExtent2D NumGobs(const LevelInfo & Info, uint32_t Level);
    static uint32_t CalculateLevelSize(const LevelInfo & Info, uint32_t Level);
    static bool IsSmallerThanGobSize(const OpenGLExtent3D & NumTiles, const OpenGLExtent2D & Gob, uint32_t BlockDepth);
    static OpenGLExtent2D GobSize(uint32_t BytesPerBlockLog2, uint32_t Height, uint32_t TileWidthSpacing);
    static uint32_t BytesPerBlockLog2(uint32_t BytesPerBlock);

    OpenGLImageType m_Type;
    SurfacePixelFormat m_Format;
    OpenGLExtent3D m_Size;
    OpenGLExtent3D m_Block;
    uint32_t m_NumSamples;
    uint32_t m_TileWidthSpacing;
    OpenGLSubresourceExtent m_Resources;
    uint32_t m_Ref;
};
