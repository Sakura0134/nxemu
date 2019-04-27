#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\Machine\Interpreter\ArmInterpreterOps64.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Debugger.h>

CPUExecutor::CPUExecutor(MemoryManagement & mmu) :
    m_Reg(this),
    m_MMU(mmu),
    m_Jumped(false)
{
}

void CPUExecutor::Execute(bool & Done)
{
    uint64_t & PROGRAM_COUNTER = m_Reg.m_PROGRAM_COUNTER;
    const bool & Stepping = isStepping();

    while (!Done)
    {
        if (Stepping)
        {
            g_Debugger->WaitForStep();
        }
        uint32_t insn;
        if (!m_MMU.Read32(PROGRAM_COUNTER, insn))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        Arm64Opcode op(PROGRAM_COUNTER, insn);

        if (!ShouldExecuteOp(op))
        {
            PROGRAM_COUNTER += 4;
            continue;
        }

        switch (op.Opc())
        {
        case Arm64Opcode::ARM64_INS_B: Arm64Op::B(*this, op); break;
        case Arm64Opcode::ARM64_INS_BL: Arm64Op::Bl(*this, op); break;
        case Arm64Opcode::ARM64_INS_CMP: Arm64Op::Cmp(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDR: Arm64Op::Ldr(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOV: Arm64Op::Mov(*this, op); break;
        case Arm64Opcode::ARM64_INS_SXTW: Arm64Op::Sxtw(*this, op); break;
        default:
            g_Settings->SaveBool(Debugger_SteppingOps, true);
            if (Stepping)
            {
                g_Debugger->WaitForStep();
            }
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (op.IsJump() || m_Jumped)
        {
            m_Jumped = false;
            continue;
        }
        PROGRAM_COUNTER += 4;    
    }
}

bool CPUExecutor::ShouldExecuteOp(const Arm64Opcode & op)
{
    if (op.cc() == Arm64Opcode::ARM64_CC_INVALID)
    {
        return true;
    }

    switch (op.Opc())
    {
    case Arm64Opcode::ARM64_INS_B: return m_Reg.ConditionSet(op.cc());
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return false;
}
