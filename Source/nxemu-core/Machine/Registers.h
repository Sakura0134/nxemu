#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\Machine\Arm64Opcode.h>

class CPUExecutor;

class CRegisters
{
    friend CPUExecutor;

public:
    CRegisters(CPUExecutor * Executor);

    uint32_t Get32(Arm64Opcode::arm64_reg reg);
    uint64_t Get64(Arm64Opcode::arm64_reg reg);

    void Set64(Arm64Opcode::arm64_reg reg, uint64_t value);

private:
    CRegisters(void);                         // Disable default constructor
    CRegisters(const CRegisters&);            // Disable copy constructor
    CRegisters& operator=(const CRegisters&); // Disable assignment

    uint64_t m_PROGRAM_COUNTER;
    uint64_t m_xregs[32];
    uint32_t * m_wregs[32];
    CPUExecutor * m_Executor;
};

