#pragma once
#include "Textures\Texture.h"
#include "Engine\Maxwell3D.h"
#include <glad\glad.h>

GLenum MaxwellToOpenGL_ComparisonOp(CMaxwell3D::ComparisonOp Comparison);
GLenum MaxwellToOpenGL_DepthCompareFunc(TextureDepthCompareFunc func);
GLenum MaxwellToOpenGL_FrontFace(CMaxwell3D::FrontFace FrontFace);
GLenum MaxwellToOpenGL_IndexFormat(CMaxwell3D::IndexFormat IndexFormat);
GLenum MaxwellToOpenGL_PolygonMode(CMaxwell3D::PolygonMode PolygonMode);
GLenum MaxwellToOpenGL_ReductionFilter(TextureSamplerReduction Filter);
GLenum MaxwellToOpenGL_StencilOp(CMaxwell3D::StencilOp Stencil);
GLenum MaxwellToOpenGL_TextureFilterMode(TextureFilter FilterMode, TextureMipmapFilter MipmapFilterMode);
GLenum MaxwellToOpenGL_PrimitiveTopology(CMaxwell3D::PrimitiveTopology Topology);
GLenum MaxwellToOpenGL_VertexFormat(CMaxwell3D::tyVertexAttribute Attrib);
GLenum MaxwellToOpenGL_ViewportSwizzle(CMaxwell3D::ViewportSwizzle Swizzle);
GLenum MaxwellToOpenGL_WrapMode(TextureWrapMode WrapMode);
