#pragma once
#include <stdint.h>
#include <vector>

enum OpenGLImageViewType
{
    OpenGLImageViewType_e1D,
    OpenGLImageViewType_e2D,
    OpenGLImageViewType_Cube,
    OpenGLImageViewType_e3D,
    OpenGLImageViewType_e1DArray,
    OpenGLImageViewType_e2DArray,
    OpenGLImageViewType_CubeArray,
    OpenGLImageViewType_Rect,
    OpenGLImageViewType_Buffer,
    OpenGLImageViewType_Last
};

enum OpenGLOptions
{
    OpenGLOptions_Size = 1 << 0,
    OpenGLOptions_Format = 1 << 1,
    OpenGLOptions_Samples = 1 << 2,
};

class OpenGLExtent2D
{
public:
    OpenGLExtent2D();
    OpenGLExtent2D(const OpenGLExtent2D & Extent);
    OpenGLExtent2D(uint32_t Width, uint32_t Height);
    OpenGLExtent2D& operator=(const OpenGLExtent2D & Extent);

    uint32_t Width(void) const { return m_Width; }
    uint32_t Height(void) const { return m_Height; }

    void Width(uint32_t Width) { m_Width = Width; }
    void Height(uint32_t Height) { m_Height = Height; }

private:
    uint32_t m_Width;
    uint32_t m_Height;
};

class OpenGLExtent3D
{
public:
    OpenGLExtent3D();
    OpenGLExtent3D(const OpenGLExtent3D & Extent);
    OpenGLExtent3D(uint32_t Width, uint32_t Height, uint32_t Depth);
    OpenGLExtent3D & operator=(const OpenGLExtent3D & Extent);

    uint32_t Width(void) const { return m_Width; }
    uint32_t Height(void) const { return m_Height; }
    uint32_t Depth(void) const { return m_Depth; }

    void Width(uint32_t Width) { m_Width = Width; }
    void Height(uint32_t Height) { m_Height = Height; }
    void Depth(uint32_t Depth) { m_Depth = Depth; }

private:
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_Depth;
};

class OpenGLSubresourceExtent
{
public:
    OpenGLSubresourceExtent();
    OpenGLSubresourceExtent(const OpenGLSubresourceExtent&);
    OpenGLSubresourceExtent(int32_t Levels, int32_t Layers);

    int32_t Levels(void) const { return m_Levels; }
    int32_t Layers(void) const { return m_Layers; }

    void Levels(uint32_t Levels) { m_Levels = Levels; }
    void Layers(uint32_t Layers) { m_Layers = Layers; }

private:
    OpenGLSubresourceExtent& operator=(const OpenGLSubresourceExtent&);

    int32_t m_Levels;
    int32_t m_Layers;
};

class OpenGLSubresourceBase
{
public:
    OpenGLSubresourceBase();
    OpenGLSubresourceBase(const OpenGLSubresourceBase&);
    OpenGLSubresourceBase(int32_t Level, int32_t Layer);
    OpenGLSubresourceBase& operator=(const OpenGLSubresourceBase&);

    int32_t Level(void) const { return m_Level; }
    int32_t Layer(void) const { return m_Layer; }

    void Level(int32_t Level) { m_Level = Level; }
    void Layer(int32_t Layers) { m_Layer = Layers; }

private:
    int32_t m_Level;
    int32_t m_Layer;
};

class OpenGLSubresourceRange
{
public:
    OpenGLSubresourceRange();
    OpenGLSubresourceRange(const OpenGLSubresourceRange & Range);
    OpenGLSubresourceRange(const OpenGLSubresourceBase & Base, const OpenGLSubresourceExtent & Extent);

    OpenGLSubresourceBase & Base(void) { return m_Base; }
    const OpenGLSubresourceBase & Base(void) const { return m_Base; }
    OpenGLSubresourceExtent & Extent(void) { return m_Extent; }
    const OpenGLSubresourceExtent & Extent(void) const { return m_Extent; }

private:
    OpenGLSubresourceRange & operator=(const OpenGLSubresourceRange &);

    OpenGLSubresourceBase m_Base;
    OpenGLSubresourceExtent m_Extent;
};

class OpenGLBufferImage
{
public:
    OpenGLBufferImage();

    uint64_t BufferOffset( void ) const { return m_BufferOffset; }
    uint32_t BufferRowLength( void ) const { return m_BufferRowLength; }
    uint32_t BufferImageHeight( void ) const { return m_BufferImageHeight; }
    int32_t ImageBaseLevel( void ) const { return m_ImageBaseLevel; }
    int32_t ImageBaseLayer( void ) const { return m_ImageBaseLayer; }
    int32_t ImageNumLayers( void ) const { return m_ImageNumLayers; }
    int32_t ImageOffsetX( void ) const { return m_ImageOffsetX; }
    int32_t ImageOffsetY( void ) const { return m_ImageOffsetY; }
    int32_t ImageOffsetZ( void ) const { return m_ImageOffsetZ; }
    const OpenGLExtent3D & ImageExtent( void ) const { return m_ImageExtent; }

    void BufferOffset(uint64_t BufferOffset) { m_BufferOffset = BufferOffset; }
    void BufferSize(uint64_t BufferSize) { m_BufferSize = BufferSize; }
    void BufferRowLength(uint32_t BufferRowLength) { m_BufferRowLength = BufferRowLength; }
    void BufferImageHeight(uint32_t BufferImageHeight) { m_BufferImageHeight = BufferImageHeight; }
    void ImageBaseLevel(int32_t ImageBaseLevel) { m_ImageBaseLevel = ImageBaseLevel; }
    void ImageBaseLayer(int32_t ImageBaseLayer) { m_ImageBaseLayer = ImageBaseLayer; }
    void ImageNumLayers(int32_t ImageNumLayers) { m_ImageNumLayers = ImageNumLayers; }
    void ImageOffsetX(int32_t ImageOffsetX) { m_ImageOffsetX = ImageOffsetX; }
    void ImageOffsetY(int32_t ImageOffsetY) { m_ImageOffsetY = ImageOffsetY; }
    void ImageOffsetZ(int32_t ImageOffsetZ) { m_ImageOffsetZ = ImageOffsetZ; }
    void ImageExtent(const OpenGLExtent3D & ImageExtent) { m_ImageExtent = ImageExtent; }

private:
    OpenGLBufferImage(const OpenGLBufferImage&);
    OpenGLBufferImage& operator=(const OpenGLBufferImage&);

    uint64_t m_BufferOffset;
    uint64_t m_BufferSize;
    uint32_t m_BufferRowLength;
    uint32_t m_BufferImageHeight;
    int32_t m_ImageBaseLevel;
    int32_t m_ImageBaseLayer;
    int32_t m_ImageNumLayers;
    int32_t m_ImageOffsetX;
    int32_t m_ImageOffsetY;
    int32_t m_ImageOffsetZ;
    OpenGLExtent3D m_ImageExtent;
};

typedef std::vector<OpenGLBufferImage> OpenGLBufferImageList;
