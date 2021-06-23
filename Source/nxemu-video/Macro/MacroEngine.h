#pragma once
#include <stdint.h>
#include <vector>
#include <memory>

typedef std::vector<uint32_t> MacroParams;

__interface MacroEngine 
{
    void AddCode(uint32_t Method, uint32_t Data);
    void Execute(uint32_t Method, const MacroParams & Params);
};

class CMaxwell3D;
std::unique_ptr<MacroEngine> GetMacroEngine(CMaxwell3D & Maxwell3D);
