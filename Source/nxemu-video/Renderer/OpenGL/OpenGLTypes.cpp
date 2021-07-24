#pragma once
#include "OpenGLTypes.h"

OpenGLExtent2D::OpenGLExtent2D() : 
    m_Width(0),
    m_Height(0)
{
}

OpenGLExtent2D::OpenGLExtent2D(uint32_t Width, uint32_t Height) : 
    m_Width(Width),
    m_Height(Height)
{
}

OpenGLExtent2D::OpenGLExtent2D(const OpenGLExtent2D & Extent) : 
    m_Width(Extent.m_Width),
    m_Height(Extent.m_Height)
{
}

OpenGLExtent2D & OpenGLExtent2D::operator=(const OpenGLExtent2D & Extent) 
{
    m_Width = Extent.m_Width;
    m_Height = Extent.m_Height;
    return *this;
}

OpenGLExtent3D::OpenGLExtent3D() : 
    m_Width(0),
    m_Height(0),
    m_Depth(0)
{
}

OpenGLExtent3D::OpenGLExtent3D(uint32_t Width, uint32_t Height, uint32_t Depth) : 
    m_Width(Width),
    m_Height(Height),
    m_Depth(Depth)
{
}

OpenGLExtent3D::OpenGLExtent3D(const OpenGLExtent3D & Extent) : 
    m_Width(Extent.m_Width),
    m_Height(Extent.m_Height),
    m_Depth(Extent.m_Depth)
{
}

OpenGLExtent3D & OpenGLExtent3D::operator=(const OpenGLExtent3D & Extent) 
{
    m_Width = Extent.m_Width;
    m_Height = Extent.m_Height;
    m_Depth = Extent.m_Depth;
    return *this;
}

OpenGLSubresourceExtent::OpenGLSubresourceExtent() :
    m_Levels(1), 
    m_Layers(1) 
{
}

OpenGLSubresourceExtent::OpenGLSubresourceExtent(const OpenGLSubresourceExtent & Extent) :
    m_Levels(Extent.m_Levels), 
    m_Layers(Extent.m_Layers) 
{
}

OpenGLSubresourceExtent::OpenGLSubresourceExtent(int32_t Levels, int32_t Layers) :
    m_Levels(Levels), 
    m_Layers(Layers) 
{
}

OpenGLSubresourceBase::OpenGLSubresourceBase() : 
    m_Level(0),
    m_Layer(0)
{
}

OpenGLSubresourceBase::OpenGLSubresourceBase(const OpenGLSubresourceBase & Base) : 
    m_Level(Base.m_Level),
    m_Layer(Base.m_Layer)
{
}

OpenGLSubresourceRange::OpenGLSubresourceRange()
{
}

OpenGLSubresourceRange::OpenGLSubresourceRange(const OpenGLSubresourceBase & Base, const OpenGLSubresourceExtent & Extent) :
    m_Base(Base),
    m_Extent(Extent)
{
}

OpenGLSubresourceRange::OpenGLSubresourceRange(const OpenGLSubresourceRange & Range) :
    m_Base(Range.m_Base),
    m_Extent(Range.m_Extent)
{
}

OpenGLBufferImage::OpenGLBufferImage() : 
    m_BufferOffset(0),
    m_BufferSize(0),
    m_BufferRowLength(0),
    m_BufferImageHeight(0),
    m_ImageBaseLevel(0),
    m_ImageBaseLayer(0),
    m_ImageNumLayers(1),
    m_ImageOffsetX(0),
    m_ImageOffsetY(0), 
    m_ImageOffsetZ(0)
{
}
