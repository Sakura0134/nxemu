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
