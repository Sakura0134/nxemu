#pragma once
#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\Settings\Debug.h>

class CSwitchSystem;
class CSystemThreadMemory;
class CSystemThread;

class CInterpreterCPU :
    public CPUExecutor,
    protected CDebugSettings
{
public:
    CInterpreterCPU(CSwitchSystem & System, CSystemThread & Thread);

    IRegisters & Reg(void) { return m_Reg; }
    CSystemThreadMemory & ThreadMemory(void) { return m_ThreadMemory; }
    CSystemThread & Thread(void) { return m_Thread; }

    void Execute(void);
    void Jump(uint64_t PC);
    uint64_t GetCycleCount(void) const;

private:
    CInterpreterCPU();
    CInterpreterCPU(const CInterpreterCPU&);
    CInterpreterCPU& operator=(const CInterpreterCPU&);

    bool ShouldExecuteOp(const Arm64Opcode & op);

    CSwitchSystem & m_System;
    CSystemThreadMemory & m_ThreadMemory;
    CSystemThread & m_Thread;
    Arm64OpcodeCache m_OpcodeCache;
    CRegisters m_Reg;
    uint64_t m_JumpPC;
    bool m_Jump;
};
