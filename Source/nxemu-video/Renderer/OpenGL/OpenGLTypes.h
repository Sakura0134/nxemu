#pragma once
#include <stdint.h>

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
