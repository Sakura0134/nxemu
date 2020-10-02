#pragma once
#include <nxemu-core\Settings\Debug.h>
#include <nxemu-core\Machine\Registers.h>
#include <nxemu-core\Machine\CPU\MemoryManagement.h>

class CInterpreterCPU :
    protected CDebugSettings
{
public:
    CInterpreterCPU(MemoryManagement & mmu);

    virtual void ServiceCall(uint32_t index) = 0;

    void Execute(bool & Done);
    void Jumped(void);

    IRegisters & Reg(void) { return m_Reg; }
    MemoryManagement & MMU(void) { return m_MMU; }
    uint64_t GetCycleCount(void) const;

protected:
    bool ShouldExecuteOp(const Arm64Opcode & op);

    CRegisters m_Reg;
    MemoryManagement & m_MMU;
    bool m_Jumped;
    Arm64OpcodeCache m_OpcodeCache;
    uint64_t m_CpuTicks;

private:
    CInterpreterCPU();
    CInterpreterCPU(const CInterpreterCPU&);
    CInterpreterCPU& operator=(const CInterpreterCPU&);
};
