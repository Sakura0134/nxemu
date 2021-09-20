#pragma once
#include <vector>

#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
union ShaderInstruction 
{
    uint64_t Value;
};
#pragma warning(pop)

typedef std::vector<ShaderInstruction> ShaderProgramCode;
