#pragma once

#include "MacroEngine.h"

class CMaxwell3D;

class CMacroInterpreter : 
    public MacroEngine 
{
public:
    CMacroInterpreter(CMaxwell3D & maxwell3d);

    void AddCode(uint32_t Method, uint32_t Data);
    void Execute(uint32_t Method, const MacroParams & Params);

private:
    CMacroInterpreter();
    CMacroInterpreter(const CMacroInterpreter&);
    CMacroInterpreter& operator=(const CMacroInterpreter&);
    
    
    CMaxwell3D & m_Maxwell3d;
};