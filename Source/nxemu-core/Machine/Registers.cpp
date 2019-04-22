#include <nxemu-core\Machine\Registers.h>
#include <nxemu-core\SystemGlobals.h>

CRegisters::CRegisters(CPUExecutor * Executor) :
    m_PROGRAM_COUNTER(0),
    m_Executor(Executor)
{
}

uint64_t CRegisters::Get64(Arm64Opcode::arm64_reg reg)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_PC: return m_PROGRAM_COUNTER;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return 0;
}

void CRegisters::Set64(Arm64Opcode::arm64_reg reg, uint64_t value)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_PC: m_PROGRAM_COUNTER = value; break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}