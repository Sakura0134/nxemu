#include <nxemu-core\Machine\Interpreter\InterpreterCPU.h>
#include <nxemu-core\Machine\Interpreter\ArmInterpreterOps64.h>
#include <nxemu-core\Machine\SystemTiming.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Debugger.h>


CInterpreterCPU::CInterpreterCPU(CSwitchSystem & System, CSystemThread & Thread) :
    m_System(System),
    m_ThreadMemory(Thread.ThreadMemory()),
    m_Thread(Thread),
    m_JumpPC(0),
    m_Jump(false)
{
}

void CInterpreterCPU::Execute()
{
    uint64_t & PROGRAM_COUNTER = m_Reg.m_PROGRAM_COUNTER;
    bool & Jump = m_Jump;
    const uint64_t & JumpPC = m_JumpPC;
    const bool & Done = m_System.EndEmulation();
    const bool & Stepping = isStepping();
    Arm64OpcodeCache & OpcodeCache = m_OpcodeCache;
    uint64_t & CpuTicks = m_Thread.CpuTicks();

    while (!Done)
    {
        if (Stepping)
        {
            g_Debugger->WaitForStep();
        }
        uint32_t insn;
        if (!m_ThreadMemory.Read32(PROGRAM_COUNTER, insn))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        Arm64Opcode op(OpcodeCache, PROGRAM_COUNTER, insn);
        CpuTicks += 1;

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
        case Arm64Opcode::ARM64_INS_ASR: Arm64Op::Asr(*this, op); break;
        case Arm64Opcode::ARM64_INS_B: Arm64Op::B(*this, op); break;
        case Arm64Opcode::ARM64_INS_BFI: Arm64Op::Bfi(*this, op); break;
        case Arm64Opcode::ARM64_INS_BFXIL: Arm64Op::Bfxil(*this, op); break;
        case Arm64Opcode::ARM64_INS_BIC: Arm64Op::Bic(*this, op); break;
        case Arm64Opcode::ARM64_INS_BL: Arm64Op::Bl(*this, op); break;
        case Arm64Opcode::ARM64_INS_BLR: Arm64Op::Blr(*this, op); break;
        case Arm64Opcode::ARM64_INS_BR: Arm64Op::Br(*this, op); break;
        case Arm64Opcode::ARM64_INS_CBNZ: Arm64Op::Cbnz(*this, op); break;
        case Arm64Opcode::ARM64_INS_CBZ: Arm64Op::Cbz(*this, op); break;
        case Arm64Opcode::ARM64_INS_CCMP: Arm64Op::Ccmp(*this, op); break;
        case Arm64Opcode::ARM64_INS_CINC: Arm64Op::Cinc(*this, op); break;
        case Arm64Opcode::ARM64_INS_CLREX: Arm64Op::Clrex(*this, op); break;
        case Arm64Opcode::ARM64_INS_CLZ: Arm64Op::Clz(*this, op); break;
        case Arm64Opcode::ARM64_INS_CMN: Arm64Op::Cmn(*this, op); break;
        case Arm64Opcode::ARM64_INS_CMP: Arm64Op::Cmp(*this, op); break;
        case Arm64Opcode::ARM64_INS_CNT: Arm64Op::Cnt(*this, op); break;
        case Arm64Opcode::ARM64_INS_CSEL: Arm64Op::Csel(*this, op); break;
        case Arm64Opcode::ARM64_INS_CSET: Arm64Op::Cset(*this, op); break;
        case Arm64Opcode::ARM64_INS_CSETM: Arm64Op::Csetm(*this, op); break;
        case Arm64Opcode::ARM64_INS_CSINC: Arm64Op::Csinc(*this, op); break;
        case Arm64Opcode::ARM64_INS_CSINV: Arm64Op::Csinv(*this, op); break;
        case Arm64Opcode::ARM64_INS_DC: Arm64Op::Dc(*this, op); break;
        case Arm64Opcode::ARM64_INS_DUP: Arm64Op::Dup(*this, op); break;
        case Arm64Opcode::ARM64_INS_DMB: Arm64Op::Dmb(*this, op); break;
        case Arm64Opcode::ARM64_INS_DSB: Arm64Op::Dsb(*this, op); break;
        case Arm64Opcode::ARM64_INS_EOR: Arm64Op::Eor(*this, op); break;
        case Arm64Opcode::ARM64_INS_EXTR: Arm64Op::Extr(*this, op); break;
        case Arm64Opcode::ARM64_INS_FABS: Arm64Op::Fabs(*this, op); break;
        case Arm64Opcode::ARM64_INS_FADD: Arm64Op::Fadd(*this, op); break;
        case Arm64Opcode::ARM64_INS_FCMP: Arm64Op::Fcmp(*this, op); break;
        case Arm64Opcode::ARM64_INS_FCSEL: Arm64Op::Fcsel(*this, op); break;
        case Arm64Opcode::ARM64_INS_FCVT: Arm64Op::Fcvt(*this, op); break;
        case Arm64Opcode::ARM64_INS_FCVTZS: Arm64Op::Fcvtzs(*this, op); break;
        case Arm64Opcode::ARM64_INS_FCVTZU: Arm64Op::Fcvtzu(*this, op); break;
        case Arm64Opcode::ARM64_INS_FDIV: Arm64Op::Fdiv(*this, op); break;
        case Arm64Opcode::ARM64_INS_FMAXNM: Arm64Op::Fmaxnm(*this, op); break;
        case Arm64Opcode::ARM64_INS_FMINNM: Arm64Op::Fminnm(*this, op); break;
        case Arm64Opcode::ARM64_INS_FMOV: Arm64Op::Fmov(*this, op); break;
        case Arm64Opcode::ARM64_INS_FMUL: Arm64Op::Fmul(*this, op); break;
        case Arm64Opcode::ARM64_INS_FNEG: Arm64Op::Fneg(*this, op); break;
        case Arm64Opcode::ARM64_INS_FSQRT: Arm64Op::Fsqrt(*this, op); break;
        case Arm64Opcode::ARM64_INS_FSUB: Arm64Op::Fsub(*this, op); break;
        case Arm64Opcode::ARM64_INS_INS: Arm64Op::Ins(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDAR: Arm64Op::Ldar(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDARB: Arm64Op::Ldarb(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDARH: Arm64Op::Ldarh(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDAXR: Arm64Op::Ldaxr(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDP: Arm64Op::Ldp(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDPSW: Arm64Op::Ldpsw(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDR: Arm64Op::Ldr(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDRB: Arm64Op::Ldrb(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDRH: Arm64Op::Ldrh(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDRSB: Arm64Op::Ldrsb(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDRSW: Arm64Op::Ldrsw(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDUR: Arm64Op::Ldur(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDURB: Arm64Op::Ldurb(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDURH: Arm64Op::Ldurh(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDURSW: Arm64Op::Ldursw(*this, op); break;
        case Arm64Opcode::ARM64_INS_LDXR: Arm64Op::Ldxr(*this, op); break;
        case Arm64Opcode::ARM64_INS_LSL: Arm64Op::Lsl(*this, op); break;
        case Arm64Opcode::ARM64_INS_LSR: Arm64Op::Lsr(*this, op); break;
        case Arm64Opcode::ARM64_INS_MADD: Arm64Op::Madd(*this, op); break;
        case Arm64Opcode::ARM64_INS_MNEG: Arm64Op::Mneg(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOV: Arm64Op::Mov(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOVI: Arm64Op::Movi(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOVK: Arm64Op::Movk(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOVN: Arm64Op::Movn(*this, op); break;
        case Arm64Opcode::ARM64_INS_MOVZ: Arm64Op::Movz(*this, op); break;
        case Arm64Opcode::ARM64_INS_MRS: Arm64Op::Mrs(*this, op); break;
        case Arm64Opcode::ARM64_INS_MSR: Arm64Op::Msr(*this, op); break;
        case Arm64Opcode::ARM64_INS_MSUB: Arm64Op::Msub(*this, op); break;
        case Arm64Opcode::ARM64_INS_MUL: Arm64Op::Mul(*this, op); break;
        case Arm64Opcode::ARM64_INS_MVN: Arm64Op::Mvn(*this, op); break;
        case Arm64Opcode::ARM64_INS_NEG: Arm64Op::Neg(*this, op); break;
        case Arm64Opcode::ARM64_INS_ORN: Arm64Op::Orn(*this, op); break;
        case Arm64Opcode::ARM64_INS_ORR: Arm64Op::Orr(*this, op); break;
        case Arm64Opcode::ARM64_INS_RBIT: Arm64Op::Rbit(*this, op); break;
        case Arm64Opcode::ARM64_INS_RET: Arm64Op::Ret(*this, op); break;
        case Arm64Opcode::ARM64_INS_ROR: Arm64Op::Ror(*this, op); break;
        case Arm64Opcode::ARM64_INS_SBFIZ: Arm64Op::Sbfiz(*this, op); break;
        case Arm64Opcode::ARM64_INS_SBFX: Arm64Op::Sbfx(*this, op); break;
        case Arm64Opcode::ARM64_INS_SCVTF: Arm64Op::Scvtf(*this, op); break;
        case Arm64Opcode::ARM64_INS_SDIV: Arm64Op::Sdiv(*this, op); break;
        case Arm64Opcode::ARM64_INS_SMULH: Arm64Op::Smulh(*this, op); break;
        case Arm64Opcode::ARM64_INS_SMADDL: Arm64Op::Smaddl(*this, op); break;
        case Arm64Opcode::ARM64_INS_SMULL: Arm64Op::Smull(*this, op); break;
        case Arm64Opcode::ARM64_INS_STLR: Arm64Op::Stlr(*this, op); break;
        case Arm64Opcode::ARM64_INS_STLRB: Arm64Op::Stlrb(*this, op); break;
        case Arm64Opcode::ARM64_INS_STLRH: Arm64Op::Stlrh(*this, op); break;
        case Arm64Opcode::ARM64_INS_STLXR: Arm64Op::Stlxr(*this, op); break;
        case Arm64Opcode::ARM64_INS_STP: Arm64Op::Stp(*this, op); break;
        case Arm64Opcode::ARM64_INS_STR: Arm64Op::Str(*this, op); break;
        case Arm64Opcode::ARM64_INS_STRB: Arm64Op::Strb(*this, op); break;
        case Arm64Opcode::ARM64_INS_STRH: Arm64Op::Strh(*this, op); break;
        case Arm64Opcode::ARM64_INS_STUR: Arm64Op::Stur(*this, op); break;
        case Arm64Opcode::ARM64_INS_STURB: Arm64Op::Sturb(*this, op); break;
        case Arm64Opcode::ARM64_INS_STURH: Arm64Op::Sturh(*this, op); break;
        case Arm64Opcode::ARM64_INS_STXR: Arm64Op::Stxr(*this, op); break;
        case Arm64Opcode::ARM64_INS_SUB: Arm64Op::Sub(*this, op); break;
        case Arm64Opcode::ARM64_INS_SVC: Arm64Op::Svc(*this, op); break;
        case Arm64Opcode::ARM64_INS_SXTW: Arm64Op::Sxtw(*this, op); break;
        case Arm64Opcode::ARM64_INS_TBNZ: Arm64Op::Tbnz(*this, op); break;
        case Arm64Opcode::ARM64_INS_TBZ: Arm64Op::Tbz(*this, op); break;
        case Arm64Opcode::ARM64_INS_TST: Arm64Op::Tst(*this, op); break;
        case Arm64Opcode::ARM64_INS_UADDLV: Arm64Op::Uaddlv(*this, op); break;
        case Arm64Opcode::ARM64_INS_UBFIZ: Arm64Op::Ubfiz(*this, op); break;
        case Arm64Opcode::ARM64_INS_UBFX: Arm64Op::Ubfx(*this, op); break;
        case Arm64Opcode::ARM64_INS_UCVTF: Arm64Op::Ucvtf(*this, op); break;
        case Arm64Opcode::ARM64_INS_UDIV: Arm64Op::Udiv(*this, op); break;
        case Arm64Opcode::ARM64_INS_UMADDL: Arm64Op::Umaddl(*this, op); break;
        case Arm64Opcode::ARM64_INS_UMULH: Arm64Op::Umulh(*this, op); break;
        case Arm64Opcode::ARM64_INS_UMULL: Arm64Op::Umull(*this, op); break;
        default:
            g_Settings->SaveBool(Debugger_SteppingOps, true);
            if (Stepping)
            {
                g_Debugger->WaitForStep();
            }
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Jump)
        {
            Jump = false;
            PROGRAM_COUNTER = JumpPC;
        }
        else
        {
            PROGRAM_COUNTER += 4;
        }
    }
}

void CInterpreterCPU::Jump(uint64_t PC)
{
    m_JumpPC = PC;
    m_Jump = true;
}

uint64_t CInterpreterCPU::GetCycleCount(void) const
{
    return CSystemTiming::CpuCyclesToClockCycles(m_Thread.CpuTicks());
}

bool CInterpreterCPU::ShouldExecuteOp(const Arm64Opcode & op)
{
    if (op.cc() == Arm64Opcode::ARM64_CC_INVALID)
    {
        return true;
    }

    switch (op.Opc())
    {
    case Arm64Opcode::ARM64_INS_B: return m_Reg.ConditionSet(op.cc());
    case Arm64Opcode::ARM64_INS_CCMP: return true;
    case Arm64Opcode::ARM64_INS_CINC: return true;
    case Arm64Opcode::ARM64_INS_CSEL: return true;
    case Arm64Opcode::ARM64_INS_CSET: return true;
    case Arm64Opcode::ARM64_INS_CSETM: return true;
    case Arm64Opcode::ARM64_INS_CSINC: return true;
    case Arm64Opcode::ARM64_INS_CSINV: return true;
    case Arm64Opcode::ARM64_INS_FCSEL: return true;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return false;
}

