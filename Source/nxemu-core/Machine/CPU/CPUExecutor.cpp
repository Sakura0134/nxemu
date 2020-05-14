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
    Arm64OpcodeCache & OpcodeCache = m_OpcodeCache;

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
        Arm64Opcode op(OpcodeCache, PROGRAM_COUNTER, insn);

        if (!ShouldExecuteOp(op))
        {
            PROGRAM_COUNTER += 4;
            continue;
        }

        switch (op.Opc())
        {
        case Arm64Opcode::ARM64_INS_ADD: Arm64Op::Add(*this, op); break;
        case Arm64Opcode::ARM64_INS_ADRP: Arm64Op::Adrp(*this, op); break;
        case Arm64Opcode::ARM64_INS_AND: Arm64Op::And(*this, op); break;
        case Arm64Opcode::ARM64_INS_B: Arm64Op::B(*this, op); break;
        case Arm64Opcode::ARM64_INS_BFI: Arm64Op::Bfi(*this, op); break;
        case Arm64Opcode::ARM64_INS_BIC: Arm64Op::Bic(*this, op); break;
        case Arm64Opcode::ARM64_INS_BL: Arm64Op::Bl(*this, op); break;
        case Arm64Opcode::ARM64_INS_BR: Arm64Op::Br(*this, op); break;
        case Arm64Opcode::ARM64_INS_CBNZ: Arm64Op::Cbnz(*this, op); break;
        case Arm64Opcode::ARM64_INS_CBZ: Arm64Op::Cbz(*this, op); break;
        case Arm64Opcode::ARM64_INS_CCMP: Arm64Op::Ccmp(*this, op); break;
        case Arm64Opcode::ARM64_INS_CLZ: Arm64Op::Clz(*this, op); break;
        case Arm64Opcode::ARM64_INS_CMN: Arm64Op::Cmn(*this, op); break;
        case Arm64Opcode::ARM64_INS_CMP: Arm64Op::Cmp(*this, op); break;
        case Arm64Opcode::ARM64_INS_CSEL: Arm64Op::Csel(*this, op); break;
        case Arm64Opcode::ARM64_INS_CSET: Arm64Op::Cset(*this, op); break;
        case Arm64Opcode::ARM64_INS_DUP: Arm64Op::Dup(*this, op); break;
        case Arm64Opcode::ARM64_INS_EOR: Arm64Op::Eor(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDP: Arm64Op::Ldp(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDR: Arm64Op::Ldr(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDRB: Arm64Op::Ldrb(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDRH: Arm64Op::Ldrh(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDRSW: Arm64Op::Ldrsw(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDUR: Arm64Op::Ldur(*this, op); break;
        case Arm64Opcode::ARM64_INS_LSL: Arm64Op::Lsl(*this, op); break;
        case Arm64Opcode::ARM64_INS_LSR: Arm64Op::Lsr(*this, op); break;
        case Arm64Opcode::ARM64_INS_MADD: Arm64Op::Madd(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOV: Arm64Op::Mov(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOVK: Arm64Op::Movk(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOVN: Arm64Op::Movn(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOVZ: Arm64Op::Movz(*this, op); break;
        case Arm64Opcode::ARM64_INS_MSUB: Arm64Op::Msub(*this, op); break;
        case Arm64Opcode::ARM64_INS_MUL: Arm64Op::Mul(*this, op); break;
        case Arm64Opcode::ARM64_INS_NEG: Arm64Op::Neg(*this, op); break;
        case Arm64Opcode::ARM64_INS_ORR: Arm64Op::Orr(*this, op); break;
        case Arm64Opcode::ARM64_INS_RBIT: Arm64Op::Rbit(*this, op); break;
        case Arm64Opcode::ARM64_INS_RET: Arm64Op::Ret(*this, op); break;
        case Arm64Opcode::ARM64_INS_STP: Arm64Op::Stp(*this, op); break;
        case Arm64Opcode::ARM64_INS_STR: Arm64Op::Str(*this, op); break;
        case Arm64Opcode::ARM64_INS_STRB: Arm64Op::Strb(*this, op); break;
        case Arm64Opcode::ARM64_INS_STUR: Arm64Op::Stur(*this, op); break;
        case Arm64Opcode::ARM64_INS_STURB: Arm64Op::Sturb(*this, op); break;
        case Arm64Opcode::ARM64_INS_SUB: Arm64Op::Sub(*this, op); break;
        case Arm64Opcode::ARM64_INS_SVC: Arm64Op::Svc(*this, op); break;
        case Arm64Opcode::ARM64_INS_SXTW: Arm64Op::Sxtw(*this, op); break;
        case Arm64Opcode::ARM64_INS_TBZ: Arm64Op::Tbz(*this, op); break;
        case Arm64Opcode::ARM64_INS_TST: Arm64Op::Tst(*this, op); break;
        case Arm64Opcode::ARM64_INS_UDIV: Arm64Op::Udiv(*this, op); break;
        case Arm64Opcode::ARM64_INS_UMADDL: Arm64Op::Umaddl(*this, op); break;
        case Arm64Opcode::ARM64_INS_UMULH: Arm64Op::Umulh(*this, op); break;
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

void CPUExecutor::Jumped(void)
{
    m_Jumped = true;
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
    case Arm64Opcode::ARM64_INS_CCMP: return true;
    case Arm64Opcode::ARM64_INS_CSEL: return true;
    case Arm64Opcode::ARM64_INS_CSET: return true;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return false;
}
