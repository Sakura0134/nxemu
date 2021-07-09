#pragma once
#include <glad/glad.h>
#include "Engine/Maxwell3D.h"

GLenum MaxwellToOpenGL_ComparisonOp(CMaxwell3D::ComparisonOp Comparison);
GLenum MaxwellToOpenGL_StencilOp(CMaxwell3D::StencilOp Stencil);