#include "Shader.h"
#include "VideoMemoryManager.h"
#include "VideoNotification.h"
#include <algorithm>

std::size_t CalculateProgramSize(const ShaderProgramCode & Code, bool IsCompute) 
{
    enum : uint64_t
    {
        SELF_JUMPING_BRANCH = 0xE2400FFFFF07000FULL,
        MASK = 0xFFFFFFFFFF7FFFFFULL,
    };

    std::size_t StartOffset = IsCompute ? SHADER_KERNEL_MAIN_OFFSET : SHADER_STAGE_MAIN_OFFSET;
    for (std::size_t i = StartOffset, n = Code.size(); i < n; i++) 
    {
        const ShaderInstruction & Instruction = Code[i];
        if (IsShaderSchedInstruction(i, StartOffset))
        {
            continue;
        }
        if ((Instruction.Value & MASK) == SELF_JUMPING_BRANCH || Instruction.Value == 0)
        {
            return std::min(i + 1, Code.size());
        }
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return Code.size();
}

ShaderProgramCode GetShaderCode(CVideoMemory & VideoMemory, uint64_t GpuAddr, bool IsCompute) 
{
    enum 
    {
        MAX_PROGRAM_LENGTH = 0x1000
    };

    ShaderProgramCode Code(MAX_PROGRAM_LENGTH);
    VideoMemory.ReadBuffer(GpuAddr, Code.data(), Code.size() * sizeof(uint64_t));
    Code.resize(CalculateProgramSize(Code, IsCompute));
    return Code;
}

bool IsShaderSchedInstruction(size_t Offset, size_t MainOffset) 
{
    enum 
    {
        SchedPeriod = 4
    };
    return ((Offset - MainOffset) % SchedPeriod) == 0;
}

uint64_t ShaderUniqueIdentifier(const ShaderProgramCode & Code, const ShaderProgramCode & CodeB, bool IncludeB) 
{
    uint64_t UniqueIdentifier = Code.size();
    for (size_t i = 0, n = Code.size(); i < n; i++)
    {
        UniqueIdentifier ^= Code[i].Value + 0x9e3779b9 + (UniqueIdentifier << 6) + (UniqueIdentifier >> 2);
    }
    if (IncludeB)
    {
        for (size_t i = 0, n = CodeB.size(); i < n; i++)
        {
            UniqueIdentifier ^= CodeB[i].Value + 0x9e3779b9 + (UniqueIdentifier << 6) + (UniqueIdentifier >> 2);
        }
    }
    return UniqueIdentifier;
}
