#pragma once
#include "Engine/Maxwell3D.h"
#include <glad/glad.h>

GLenum MaxwellToOpenGL_ComparisonOp(CMaxwell3D::ComparisonOp Comparison);
GLenum MaxwellToOpenGL_FrontFace(CMaxwell3D::FrontFace FrontFace);
GLenum MaxwellToOpenGL_PolygonMode(CMaxwell3D::PolygonMode PolygonMode);
GLenum MaxwellToOpenGL_StencilOp(CMaxwell3D::StencilOp Stencil);
GLenum MaxwellToOpenGL_VertexFormat(CMaxwell3D::tyVertexAttribute Attrib);
GLenum MaxwellToOpenGL_ViewportSwizzle(CMaxwell3D::ViewportSwizzle Swizzle);
