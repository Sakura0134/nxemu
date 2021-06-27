#pragma once

#include "MacroEngine.h"
#include "Macro.h"
#include <unordered_map>
#include <vector>

class CMaxwell3D;

class CMacroInterpreter : 
    public MacroEngine 
{
    typedef std::vector<uint32_t> CodeList;

public:
    CMacroInterpreter(CMaxwell3D & maxwell3d);

    void AddCode(uint32_t Method, uint32_t Data);
    void Execute(uint32_t Method, const MacroParams & Params);

private:
    CMacroInterpreter();
    CMacroInterpreter(const CMacroInterpreter&);
    CMacroInterpreter& operator=(const CMacroInterpreter&);
    
    uint32_t GetALUResult(MacroALUOperation Operation, uint32_t SrcA, uint32_t SrcB);
    void ProcessResult(const MacroParams & Params, MacroResultOperation Operation, uint32_t Reg, uint32_t Result);
    void SetRegister(uint32_t Reg, uint32_t Value);
    void Send(uint32_t Value); 
    
    CMaxwell3D & m_Maxwell3d;
    uint32_t m_Registers[8];
    MacroMethodAddress m_MethodAddress;
    uint32_t m_NextParam;
    std::unordered_map<uint32_t, CodeList> m_MacroCode;
    bool m_Carry;
};