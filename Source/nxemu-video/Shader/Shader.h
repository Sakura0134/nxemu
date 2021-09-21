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

enum ShaderPredicate : uint64_t
{
    ShaderPredicate_UnusedIndex = 0x7,
    ShaderPredicate_NeverExecute = 0xF,
};

enum ShaderInternalFlag
{
    ShaderInternalFlag_Zero = 0, 
    ShaderInternalFlag_Sign = 1,
    ShaderInternalFlag_Carry = 2,
    ShaderInternalFlag_Overflow = 3, 
    ShaderInternalFlag_Amount = 4,
};

enum
{
    SHADER_KERNEL_MAIN_OFFSET = 0,
    SHADER_STAGE_MAIN_OFFSET = 10,
    SHADER_MAX_PROGRAM_LENGTH = 0x1000,
};

ShaderProgramCode GetShaderCode(CVideoMemory & VideoMemory, uint64_t GpuAddr, bool IsCompute);
bool IsShaderSchedInstruction(size_t Offset, size_t MainOffset);
uint64_t ShaderUniqueIdentifier(const ShaderProgramCode & Code, const ShaderProgramCode & CodeB, bool IncludeB);
