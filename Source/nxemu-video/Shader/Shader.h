#pragma once
#include "ShaderInstruction.h"
#include <stdint.h>

class CVideoMemory;

enum ShaderType : unsigned 
{
    ShaderType_Vertex = 0,
    ShaderType_TesselationControl = 1,
    ShaderType_TesselationEval = 2,
    ShaderType_Geometry = 3,
    ShaderType_Fragment = 4,
    ShaderType_Compute = 5,
};

enum
{
    SHADER_KERNEL_MAIN_OFFSET = 0,
    SHADER_STAGE_MAIN_OFFSET = 10,
};

ShaderProgramCode GetShaderCode(CVideoMemory & VideoMemory, uint64_t GpuAddr, bool IsCompute);
bool IsShaderSchedInstruction(size_t Offset, size_t MainOffset);
uint64_t ShaderUniqueIdentifier(const ShaderProgramCode & Code, const ShaderProgramCode & CodeB, bool IncludeB);
