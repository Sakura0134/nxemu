#include <nxemu-core\Machine\Arm64Opcode.h>
#include <capstone\Include\capstone.h>
#include <nxemu-core\SystemGlobals.h>

Arm64Opcode::Arm64OpcodeDetail::Arm64OpcodeDetail(uint64_t pc, uint32_t insn) :
    m_pc(pc),
    m_WriteBack(false),
    m_Opc(ARM64_INS_INVALID),
    m_OperandCount(0),
    m_UpdateFlags(false),
    m_cc(ARM64_CC_INVALID)
{
    m_Name[0] = '\0';
    m_Param[0] = '\0';
    memset(m_Operands, 0, sizeof(m_Operands));
    csh handle;
    cs_err err = cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle);
    if (err)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

    cs_insn *results;
    size_t count = cs_disasm(handle, (uint8_t *)&insn, sizeof(insn), pc, 0, &results);
    if (count == 0)
    {
        cs_close(&handle);
        return;
    }

    m_Opc = (instruct_t)results[0].id;
    strcpy(m_Name,results[0].mnemonic);
    strcpy(m_Param,results[0].op_str);
    m_cc = (Arm64Opcode::arm64_cc)results[0].detail->arm64.cc;
    m_WriteBack = results[0].detail->arm64.writeback;
    m_UpdateFlags = results[0].detail->arm64.update_flags;
    m_OperandCount = results[0].detail->arm64.op_count;
    for (uint8_t i = 0, n = results[0].detail->arm64.op_count; i < n; i++)
    {
        cs_arm64_op & src_operand = results[0].detail->arm64.operands[i];
        MCOperand &operand = m_Operands[i];
        memset(&operand, 0, sizeof(operand));

        operand.type = (arm64_op_type)src_operand.type;
        operand.shift.type = (arm64_shifter)src_operand.shift.type;
        operand.shift.value = src_operand.shift.value;
        operand.Extend = (arm64_extender)src_operand.ext;
        operand.VectorIndex = src_operand.vector_index;
        operand.Vess = (arm64_vess)src_operand.vess;
        operand.Vas = (arm64_vas)src_operand.vas;
        switch (src_operand.type)
        {
        case ARM64_OP_REG:
            operand.Reg = TranslateArm64Reg((capstone_arm64_reg)src_operand.reg);
            break;
        case ARM64_OP_IMM:
            if (results[0].id == ARM64_INS_MOVK)
            {
                operand.ImmVal = src_operand.imm;
                operand.shift.type = (arm64_shifter)src_operand.shift.type;
                operand.shift.value = src_operand.shift.value;
            }
            else if (src_operand.shift.type == ARM64_SFT_LSL)
            {
                operand.ImmVal = src_operand.imm << src_operand.shift.value;
                operand.shift.type = ARM64_SFT_INVALID;
                operand.shift.value = 0;
            }
            else if (src_operand.shift.type != ARM64_SFT_INVALID)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            else
            {
                operand.ImmVal = src_operand.imm;
            }
            break;
        case ARM64_OP_FP:
            operand.fp = src_operand.fp;
            break;
        case ARM64_OP_MEM:
            operand.mem.base = TranslateArm64Reg((capstone_arm64_reg)src_operand.mem.base);
            operand.mem.index = TranslateArm64Reg((capstone_arm64_reg)src_operand.mem.index);
            operand.mem.disp = src_operand.mem.disp;
            break;
        case ARM64_OP_REG_MRS:
        case ARM64_OP_REG_MSR:
            if (src_operand.shift.type != ARM64_SFT_INVALID)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            operand.SysReg = (A64SysRegValues)src_operand.imm;
            break;
        case ARM64_OP_BARRIER:
        case ARM64_OP_SYS:
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    cs_free(results, count);
    cs_close(&handle);
}

Arm64Opcode::Arm64Opcode(Arm64OpcodeCache &cache, uint64_t pc, uint32_t insn) :
    m_Details(cache.GetOpcodeDetail(pc, insn))
{
}

bool Arm64Opcode::IsJump(void) const
{
    switch (Opc())
    {
    case ARM64_INS_B:
    case ARM64_INS_BL:
    case ARM64_INS_BLR:
    case ARM64_INS_BR:
    case ARM64_INS_RET:
        return true;
    case ARM64_INS_ADD:
    case ARM64_INS_ADR:
    case ARM64_INS_ADRP:
    case ARM64_INS_AND:
    case ARM64_INS_ASR:
    case ARM64_INS_BFI:
    case ARM64_INS_BFXIL:
    case ARM64_INS_BIC:
    case ARM64_INS_CCMP:
    case ARM64_INS_CINC:
    case ARM64_INS_CLREX:
    case ARM64_INS_CLZ:
    case ARM64_INS_CMN:
    case ARM64_INS_CMP:
    case ARM64_INS_CNT:
    case ARM64_INS_CSEL:
    case ARM64_INS_CSET:
    case ARM64_INS_CSETM:
    case ARM64_INS_CSINC:
    case ARM64_INS_CSINV:
    case ARM64_INS_DC:
    case ARM64_INS_DMB:
    case ARM64_INS_DSB:
    case ARM64_INS_DUP:
    case ARM64_INS_EOR:
    case ARM64_INS_EXTR:
    case ARM64_INS_FABS:
    case ARM64_INS_FADD:
    case ARM64_INS_FCMP:
    case ARM64_INS_FCSEL:
    case ARM64_INS_FCVT:
    case ARM64_INS_FCVTMS:
    case ARM64_INS_FCVTPS:
    case ARM64_INS_FCVTZS:
    case ARM64_INS_FCVTZU:
    case ARM64_INS_FDIV:
    case ARM64_INS_FMAXNM:
    case ARM64_INS_FMINNM:
    case ARM64_INS_FMOV:
    case ARM64_INS_FMUL:
    case ARM64_INS_FNEG:
    case ARM64_INS_FSQRT:
    case ARM64_INS_FSUB:
    case ARM64_INS_INS:
    case ARM64_INS_LDAR:
    case ARM64_INS_LDARB:
    case ARM64_INS_LDARH:
    case ARM64_INS_LDAXR:
    case ARM64_INS_LDP:
    case ARM64_INS_LDPSW:
    case ARM64_INS_LDR:
    case ARM64_INS_LDRB:
    case ARM64_INS_LDRH:
    case ARM64_INS_LDRSB:
    case ARM64_INS_LDRSH:
    case ARM64_INS_LDRSW:
    case ARM64_INS_LDUR:
    case ARM64_INS_LDURB:
    case ARM64_INS_LDURH:
    case ARM64_INS_LDURSW:
    case ARM64_INS_LDXR:
    case ARM64_INS_LSL:
    case ARM64_INS_LSR:
    case ARM64_INS_MADD:
    case ARM64_INS_MNEG:
    case ARM64_INS_MOV:
    case ARM64_INS_MOVI:
    case ARM64_INS_MOVK:
    case ARM64_INS_MOVN:
    case ARM64_INS_MOVZ:
    case ARM64_INS_MRS:
    case ARM64_INS_MSR:
    case ARM64_INS_MSUB:
    case ARM64_INS_MUL:
    case ARM64_INS_MVN:
    case ARM64_INS_NEG:
    case ARM64_INS_NOP:
    case ARM64_INS_ORN:
    case ARM64_INS_ORR:
    case ARM64_INS_RBIT:
    case ARM64_INS_ROR:
    case ARM64_INS_SBFIZ:
    case ARM64_INS_SBFX:
    case ARM64_INS_SCVTF:
    case ARM64_INS_SDIV:
    case ARM64_INS_SMADDL:
    case ARM64_INS_SMULH:
    case ARM64_INS_SMULL:
    case ARM64_INS_STLR:
    case ARM64_INS_STLRB:
    case ARM64_INS_STLRH:
    case ARM64_INS_STLXR:
    case ARM64_INS_STP:
    case ARM64_INS_STR:
    case ARM64_INS_STRB:
    case ARM64_INS_STRH:
    case ARM64_INS_STUR:
    case ARM64_INS_STURB:
    case ARM64_INS_STURH:
    case ARM64_INS_STXR:
    case ARM64_INS_SUB:
    case ARM64_INS_SVC:
    case ARM64_INS_SXTW:
    case ARM64_INS_TST:
    case ARM64_INS_UADDLV:
    case ARM64_INS_UBFIZ:
    case ARM64_INS_UBFX:
    case ARM64_INS_UCVTF:
    case ARM64_INS_UDIV:
    case ARM64_INS_UMADDL:
    case ARM64_INS_UMULH:
    case ARM64_INS_UMULL:
        return false;
    //Conditional jump, return false as does not always jump
    case ARM64_INS_CBNZ:
    case ARM64_INS_CBZ:
    case ARM64_INS_TBNZ:
    case ARM64_INS_TBZ:
        return false;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
}

bool Arm64Opcode::IsBranch(void) const
{
    switch (Opc())
    {
    case ARM64_INS_B:
    case ARM64_INS_BL:
        if (Operands() == 1 && Operand(0).type == Arm64Opcode::ARM64_OP_IMM)
        {
            return true;
        }
        break;
    case ARM64_INS_CBZ:
    case ARM64_INS_CBNZ:
        if (Operands() == 2 && Operand(0).type == Arm64Opcode::ARM64_OP_REG && Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            return true;
        }
        break;
    case ARM64_INS_TBNZ:
    case ARM64_INS_TBZ:
        if (Operands() == 3 && Operand(0).type == Arm64Opcode::ARM64_OP_REG && Operand(1).type == Arm64Opcode::ARM64_OP_IMM && Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            return true;
        }
        return true;
    }
    return false;
}

uint64_t Arm64Opcode::BranchDest(void) const
{
    switch (Opc())
    {
    case ARM64_INS_B:
    case ARM64_INS_BL:
        if (Operands() == 1 && Operand(0).type == Arm64Opcode::ARM64_OP_IMM)
        {
            return Operand(0).ImmVal;
        }
    case ARM64_INS_CBZ:
    case ARM64_INS_CBNZ:
        if (Operands() == 2 && Operand(0).type == Arm64Opcode::ARM64_OP_REG && Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            return Operand(1).ImmVal;
        }
        break;
    case ARM64_INS_TBNZ:
    case ARM64_INS_TBZ:
        if (Operands() == 3 && Operand(0).type == Arm64Opcode::ARM64_OP_REG && Operand(1).type == Arm64Opcode::ARM64_OP_IMM && Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            return Operand(2).ImmVal;
        }
        break;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return PC() + 4;
}

bool Arm64Opcode::Is32bitReg(arm64_reg reg)
{
    if ((reg >= ARM64_REG_W0 && reg <= ARM64_REG_W30) ||
        reg == ARM64_REG_WZR)
    {
        return true;
    }
    if ((reg >= ARM64_REG_X0 && reg <= ARM64_REG_X30) ||
        (reg >= ARM64_REG_V0 && reg <= ARM64_REG_V31) ||
        (reg >= ARM64_REG_S0 && reg <= ARM64_REG_S31) ||
        (reg >= ARM64_REG_D0 && reg <= ARM64_REG_D31) ||
        (reg >= ARM64_REG_Q0 && reg <= ARM64_REG_Q31) ||
        reg == ARM64_REG_XZR)
    {
        return false;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool Arm64Opcode::Is32bitFloatReg(arm64_reg reg)
{
    if (reg >= ARM64_REG_S0 && reg <= ARM64_REG_S31)
    {
        return true;
    }
    else if ((reg >= ARM64_REG_Q0 && reg <= ARM64_REG_Q31) ||
        (reg >= ARM64_REG_D0 && reg <= ARM64_REG_D31))
    {
        return false;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool Arm64Opcode::Is64bitReg(arm64_reg reg)
{
    if ((reg >= ARM64_REG_X0 && reg <= ARM64_REG_X30) ||
        reg == ARM64_REG_XZR ||
        reg == ARM64_REG_SP)
    {
        return true;
    }
    if ((reg >= ARM64_REG_Q0 && reg <= ARM64_REG_Q31) ||
        (reg >= ARM64_REG_W0 && reg <= ARM64_REG_W30) ||
        (reg >= ARM64_REG_V0 && reg <= ARM64_REG_V31) ||
        (reg >= ARM64_REG_D0 && reg <= ARM64_REG_D31) ||
        (reg >= ARM64_REG_S0 && reg <= ARM64_REG_S31) ||
        reg == ARM64_REG_WZR)
    {
        return false;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool Arm64Opcode::Is64bitFloatReg(arm64_reg reg)
{
    if (reg >= ARM64_REG_D0 && reg <= ARM64_REG_D31)
    {
        return true;
    }
    else if ((reg >= ARM64_REG_Q0 && reg <= ARM64_REG_Q31) ||
        (reg >= ARM64_REG_W0 && reg <= ARM64_REG_W30) ||
        (reg >= ARM64_REG_S0 && reg <= ARM64_REG_S31))
    {
        return false;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool Arm64Opcode::IsVectorReg(arm64_reg reg)
{
    if (reg >= ARM64_REG_V0 && reg <= ARM64_REG_V31)
    {
        return true;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool Arm64Opcode::Is16bitSimdReg(arm64_reg reg)
{
    if (reg >= ARM64_REG_H0 && reg <= ARM64_REG_H31)
    {
        return true;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool Arm64Opcode::Is128bitReg(arm64_reg reg)
{
    if (reg >= ARM64_REG_Q0 && reg <= ARM64_REG_Q31)
    {
        return true;
    }
    if ((reg >= ARM64_REG_X0 && reg <= ARM64_REG_X30) ||
        (reg >= ARM64_REG_W0 && reg <= ARM64_REG_W30) ||
        (reg >= ARM64_REG_D0 && reg <= ARM64_REG_D31) ||
        reg == ARM64_REG_XZR || reg == ARM64_REG_WZR)
    {
        return false;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

Arm64OpcodeCache::Arm64OpcodeCache()
{
}

Arm64OpcodeCache::~Arm64OpcodeCache()
{
}

const Arm64Opcode::Arm64OpcodeDetail & Arm64OpcodeCache::GetOpcodeDetail(uint64_t pc, uint32_t insn)
{
    OpcodeKey key{ pc,insn };
    {
        CGuard guard(m_CacheCS);
        OPCODE_CACHE::const_iterator itr = m_OpcodeCache.find(key);
        if (itr != m_OpcodeCache.end())
        {
            return itr->second;
        }
    }
    Arm64Opcode::Arm64OpcodeDetail Details(pc, insn);
    {
        CGuard guard(m_CacheCS);
        std::pair<OPCODE_CACHE::const_iterator, bool> res = m_OpcodeCache.insert(OPCODE_CACHE::value_type(key, std::move(Details)));
        if (!res.second)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        return res.first->second;
    }

    g_Notify->BreakPoint(__FILE__, __LINE__);
    static Arm64Opcode::Arm64OpcodeDetail NullDetails(0, 0);
    return NullDetails;
}

Arm64Opcode::arm64_reg Arm64Opcode::Arm64OpcodeDetail::TranslateArm64Reg(capstone_arm64_reg reg)
{
    switch (reg)
    {
    case CAPSTONE_ARM64_REG_INVALID: return ARM64_REG_INVALID;
    case CAPSTONE_ARM64_REG_X0: return ARM64_REG_X0;
    case CAPSTONE_ARM64_REG_X1: return ARM64_REG_X1;
    case CAPSTONE_ARM64_REG_X2: return ARM64_REG_X2;
    case CAPSTONE_ARM64_REG_X3: return ARM64_REG_X3;
    case CAPSTONE_ARM64_REG_X4: return ARM64_REG_X4;
    case CAPSTONE_ARM64_REG_X5: return ARM64_REG_X5;
    case CAPSTONE_ARM64_REG_X6: return ARM64_REG_X6;
    case CAPSTONE_ARM64_REG_X7: return ARM64_REG_X7;
    case CAPSTONE_ARM64_REG_X8: return ARM64_REG_X8;
    case CAPSTONE_ARM64_REG_X9: return ARM64_REG_X9;
    case CAPSTONE_ARM64_REG_X10: return ARM64_REG_X10;
    case CAPSTONE_ARM64_REG_X11: return ARM64_REG_X11;
    case CAPSTONE_ARM64_REG_X12: return ARM64_REG_X12;
    case CAPSTONE_ARM64_REG_X13: return ARM64_REG_X13;
    case CAPSTONE_ARM64_REG_X14: return ARM64_REG_X14;
    case CAPSTONE_ARM64_REG_X15: return ARM64_REG_X15;
    case CAPSTONE_ARM64_REG_X16: return ARM64_REG_X16;
    case CAPSTONE_ARM64_REG_X17: return ARM64_REG_X17;
    case CAPSTONE_ARM64_REG_X18: return ARM64_REG_X18;
    case CAPSTONE_ARM64_REG_X19: return ARM64_REG_X19;
    case CAPSTONE_ARM64_REG_X20: return ARM64_REG_X20;
    case CAPSTONE_ARM64_REG_X21: return ARM64_REG_X21;
    case CAPSTONE_ARM64_REG_X22: return ARM64_REG_X22;
    case CAPSTONE_ARM64_REG_X23: return ARM64_REG_X23;
    case CAPSTONE_ARM64_REG_X24: return ARM64_REG_X24;
    case CAPSTONE_ARM64_REG_X25: return ARM64_REG_X25;
    case CAPSTONE_ARM64_REG_X26: return ARM64_REG_X26;
    case CAPSTONE_ARM64_REG_X27: return ARM64_REG_X27;
    case CAPSTONE_ARM64_REG_X28: return ARM64_REG_X28;
    case CAPSTONE_ARM64_REG_X29: return ARM64_REG_X29;
    case CAPSTONE_ARM64_REG_X30: return ARM64_REG_X30;
    case CAPSTONE_ARM64_REG_XZR: return ARM64_REG_XZR;
    case CAPSTONE_ARM64_REG_W0: return ARM64_REG_W0;
    case CAPSTONE_ARM64_REG_W1: return ARM64_REG_W1;
    case CAPSTONE_ARM64_REG_W2: return ARM64_REG_W2;
    case CAPSTONE_ARM64_REG_W3: return ARM64_REG_W3;
    case CAPSTONE_ARM64_REG_W4: return ARM64_REG_W4;
    case CAPSTONE_ARM64_REG_W5: return ARM64_REG_W5;
    case CAPSTONE_ARM64_REG_W6: return ARM64_REG_W6;
    case CAPSTONE_ARM64_REG_W7: return ARM64_REG_W7;
    case CAPSTONE_ARM64_REG_W8: return ARM64_REG_W8;
    case CAPSTONE_ARM64_REG_W9: return ARM64_REG_W9;
    case CAPSTONE_ARM64_REG_W10: return ARM64_REG_W10;
    case CAPSTONE_ARM64_REG_W11: return ARM64_REG_W11;
    case CAPSTONE_ARM64_REG_W12: return ARM64_REG_W12;
    case CAPSTONE_ARM64_REG_W13: return ARM64_REG_W13;
    case CAPSTONE_ARM64_REG_W14: return ARM64_REG_W14;
    case CAPSTONE_ARM64_REG_W15: return ARM64_REG_W15;
    case CAPSTONE_ARM64_REG_W16: return ARM64_REG_W16;
    case CAPSTONE_ARM64_REG_W17: return ARM64_REG_W17;
    case CAPSTONE_ARM64_REG_W18: return ARM64_REG_W18;
    case CAPSTONE_ARM64_REG_W19: return ARM64_REG_W19;
    case CAPSTONE_ARM64_REG_W20: return ARM64_REG_W20;
    case CAPSTONE_ARM64_REG_W21: return ARM64_REG_W21;
    case CAPSTONE_ARM64_REG_W22: return ARM64_REG_W22;
    case CAPSTONE_ARM64_REG_W23: return ARM64_REG_W23;
    case CAPSTONE_ARM64_REG_W24: return ARM64_REG_W24;
    case CAPSTONE_ARM64_REG_W25: return ARM64_REG_W25;
    case CAPSTONE_ARM64_REG_W26: return ARM64_REG_W26;
    case CAPSTONE_ARM64_REG_W27: return ARM64_REG_W27;
    case CAPSTONE_ARM64_REG_W28: return ARM64_REG_W28;
    case CAPSTONE_ARM64_REG_W29: return ARM64_REG_W29;
    case CAPSTONE_ARM64_REG_W30: return ARM64_REG_W30;
    case CAPSTONE_ARM64_REG_WZR: return ARM64_REG_WZR;
    case CAPSTONE_ARM64_REG_Q0: return ARM64_REG_Q0;
    case CAPSTONE_ARM64_REG_Q1: return ARM64_REG_Q1;
    case CAPSTONE_ARM64_REG_Q2: return ARM64_REG_Q2;
    case CAPSTONE_ARM64_REG_Q3: return ARM64_REG_Q3;
    case CAPSTONE_ARM64_REG_Q4: return ARM64_REG_Q4;
    case CAPSTONE_ARM64_REG_Q5: return ARM64_REG_Q5;
    case CAPSTONE_ARM64_REG_Q6: return ARM64_REG_Q6;
    case CAPSTONE_ARM64_REG_Q7: return ARM64_REG_Q7;
    case CAPSTONE_ARM64_REG_Q8: return ARM64_REG_Q8;
    case CAPSTONE_ARM64_REG_Q9: return ARM64_REG_Q9;
    case CAPSTONE_ARM64_REG_Q10: return ARM64_REG_Q10;
    case CAPSTONE_ARM64_REG_Q11: return ARM64_REG_Q11;
    case CAPSTONE_ARM64_REG_Q12: return ARM64_REG_Q12;
    case CAPSTONE_ARM64_REG_Q13: return ARM64_REG_Q13;
    case CAPSTONE_ARM64_REG_Q14: return ARM64_REG_Q14;
    case CAPSTONE_ARM64_REG_Q15: return ARM64_REG_Q15;
    case CAPSTONE_ARM64_REG_Q16: return ARM64_REG_Q16;
    case CAPSTONE_ARM64_REG_Q17: return ARM64_REG_Q17;
    case CAPSTONE_ARM64_REG_Q18: return ARM64_REG_Q18;
    case CAPSTONE_ARM64_REG_Q19: return ARM64_REG_Q19;
    case CAPSTONE_ARM64_REG_Q20: return ARM64_REG_Q20;
    case CAPSTONE_ARM64_REG_Q21: return ARM64_REG_Q21;
    case CAPSTONE_ARM64_REG_Q22: return ARM64_REG_Q22;
    case CAPSTONE_ARM64_REG_Q23: return ARM64_REG_Q23;
    case CAPSTONE_ARM64_REG_Q24: return ARM64_REG_Q24;
    case CAPSTONE_ARM64_REG_Q25: return ARM64_REG_Q25;
    case CAPSTONE_ARM64_REG_Q26: return ARM64_REG_Q26;
    case CAPSTONE_ARM64_REG_Q27: return ARM64_REG_Q27;
    case CAPSTONE_ARM64_REG_Q28: return ARM64_REG_Q28;
    case CAPSTONE_ARM64_REG_Q29: return ARM64_REG_Q29;
    case CAPSTONE_ARM64_REG_Q30: return ARM64_REG_Q30;
    case CAPSTONE_ARM64_REG_Q31: return ARM64_REG_Q31;
    case CAPSTONE_ARM64_REG_V0: return ARM64_REG_V0;
    case CAPSTONE_ARM64_REG_V1: return ARM64_REG_V1;
    case CAPSTONE_ARM64_REG_V2: return ARM64_REG_V2;
    case CAPSTONE_ARM64_REG_V3: return ARM64_REG_V3;
    case CAPSTONE_ARM64_REG_V4: return ARM64_REG_V4;
    case CAPSTONE_ARM64_REG_V5: return ARM64_REG_V5;
    case CAPSTONE_ARM64_REG_V6: return ARM64_REG_V6;
    case CAPSTONE_ARM64_REG_V7: return ARM64_REG_V7;
    case CAPSTONE_ARM64_REG_V8: return ARM64_REG_V8;
    case CAPSTONE_ARM64_REG_V9: return ARM64_REG_V9;
    case CAPSTONE_ARM64_REG_V10: return ARM64_REG_V10;
    case CAPSTONE_ARM64_REG_V11: return ARM64_REG_V11;
    case CAPSTONE_ARM64_REG_V12: return ARM64_REG_V12;
    case CAPSTONE_ARM64_REG_V13: return ARM64_REG_V13;
    case CAPSTONE_ARM64_REG_V14: return ARM64_REG_V14;
    case CAPSTONE_ARM64_REG_V15: return ARM64_REG_V15;
    case CAPSTONE_ARM64_REG_V16: return ARM64_REG_V16;
    case CAPSTONE_ARM64_REG_V17: return ARM64_REG_V17;
    case CAPSTONE_ARM64_REG_V18: return ARM64_REG_V18;
    case CAPSTONE_ARM64_REG_V19: return ARM64_REG_V19;
    case CAPSTONE_ARM64_REG_V20: return ARM64_REG_V20;
    case CAPSTONE_ARM64_REG_V21: return ARM64_REG_V21;
    case CAPSTONE_ARM64_REG_V22: return ARM64_REG_V22;
    case CAPSTONE_ARM64_REG_V23: return ARM64_REG_V23;
    case CAPSTONE_ARM64_REG_V24: return ARM64_REG_V24;
    case CAPSTONE_ARM64_REG_V25: return ARM64_REG_V25;
    case CAPSTONE_ARM64_REG_V26: return ARM64_REG_V26;
    case CAPSTONE_ARM64_REG_V27: return ARM64_REG_V27;
    case CAPSTONE_ARM64_REG_V28: return ARM64_REG_V28;
    case CAPSTONE_ARM64_REG_V29: return ARM64_REG_V29;
    case CAPSTONE_ARM64_REG_V30: return ARM64_REG_V30;
    case CAPSTONE_ARM64_REG_V31: return ARM64_REG_V31;
    case CAPSTONE_ARM64_REG_D0: return ARM64_REG_D0;
    case CAPSTONE_ARM64_REG_D1: return ARM64_REG_D1;
    case CAPSTONE_ARM64_REG_D2: return ARM64_REG_D2;
    case CAPSTONE_ARM64_REG_D3: return ARM64_REG_D3;
    case CAPSTONE_ARM64_REG_D4: return ARM64_REG_D4;
    case CAPSTONE_ARM64_REG_D5: return ARM64_REG_D5;
    case CAPSTONE_ARM64_REG_D6: return ARM64_REG_D6;
    case CAPSTONE_ARM64_REG_D7: return ARM64_REG_D7;
    case CAPSTONE_ARM64_REG_D8: return ARM64_REG_D8;
    case CAPSTONE_ARM64_REG_D9: return ARM64_REG_D9;
    case CAPSTONE_ARM64_REG_D10: return ARM64_REG_D10;
    case CAPSTONE_ARM64_REG_D11: return ARM64_REG_D11;
    case CAPSTONE_ARM64_REG_D12: return ARM64_REG_D12;
    case CAPSTONE_ARM64_REG_D13: return ARM64_REG_D13;
    case CAPSTONE_ARM64_REG_D14: return ARM64_REG_D14;
    case CAPSTONE_ARM64_REG_D15: return ARM64_REG_D15;
    case CAPSTONE_ARM64_REG_D16: return ARM64_REG_D16;
    case CAPSTONE_ARM64_REG_D17: return ARM64_REG_D17;
    case CAPSTONE_ARM64_REG_D18: return ARM64_REG_D18;
    case CAPSTONE_ARM64_REG_D19: return ARM64_REG_D19;
    case CAPSTONE_ARM64_REG_D20: return ARM64_REG_D20;
    case CAPSTONE_ARM64_REG_D21: return ARM64_REG_D21;
    case CAPSTONE_ARM64_REG_D22: return ARM64_REG_D22;
    case CAPSTONE_ARM64_REG_D23: return ARM64_REG_D23;
    case CAPSTONE_ARM64_REG_D24: return ARM64_REG_D24;
    case CAPSTONE_ARM64_REG_D25: return ARM64_REG_D25;
    case CAPSTONE_ARM64_REG_D26: return ARM64_REG_D26;
    case CAPSTONE_ARM64_REG_D27: return ARM64_REG_D27;
    case CAPSTONE_ARM64_REG_D28: return ARM64_REG_D28;
    case CAPSTONE_ARM64_REG_D29: return ARM64_REG_D29;
    case CAPSTONE_ARM64_REG_D30: return ARM64_REG_D30;
    case CAPSTONE_ARM64_REG_D31: return ARM64_REG_D31;
    case CAPSTONE_ARM64_REG_S0: return ARM64_REG_S0;
    case CAPSTONE_ARM64_REG_S1: return ARM64_REG_S1;
    case CAPSTONE_ARM64_REG_S2: return ARM64_REG_S2;
    case CAPSTONE_ARM64_REG_S3: return ARM64_REG_S3;
    case CAPSTONE_ARM64_REG_S4: return ARM64_REG_S4;
    case CAPSTONE_ARM64_REG_S5: return ARM64_REG_S5;
    case CAPSTONE_ARM64_REG_S6: return ARM64_REG_S6;
    case CAPSTONE_ARM64_REG_S7: return ARM64_REG_S7;
    case CAPSTONE_ARM64_REG_S8: return ARM64_REG_S8;
    case CAPSTONE_ARM64_REG_S9: return ARM64_REG_S9;
    case CAPSTONE_ARM64_REG_S10: return ARM64_REG_S10;
    case CAPSTONE_ARM64_REG_S11: return ARM64_REG_S11;
    case CAPSTONE_ARM64_REG_S12: return ARM64_REG_S12;
    case CAPSTONE_ARM64_REG_S13: return ARM64_REG_S13;
    case CAPSTONE_ARM64_REG_S14: return ARM64_REG_S14;
    case CAPSTONE_ARM64_REG_S15: return ARM64_REG_S15;
    case CAPSTONE_ARM64_REG_S16: return ARM64_REG_S16;
    case CAPSTONE_ARM64_REG_S17: return ARM64_REG_S17;
    case CAPSTONE_ARM64_REG_S18: return ARM64_REG_S18;
    case CAPSTONE_ARM64_REG_S19: return ARM64_REG_S19;
    case CAPSTONE_ARM64_REG_S20: return ARM64_REG_S20;
    case CAPSTONE_ARM64_REG_S21: return ARM64_REG_S21;
    case CAPSTONE_ARM64_REG_S22: return ARM64_REG_S22;
    case CAPSTONE_ARM64_REG_S23: return ARM64_REG_S23;
    case CAPSTONE_ARM64_REG_S24: return ARM64_REG_S24;
    case CAPSTONE_ARM64_REG_S25: return ARM64_REG_S25;
    case CAPSTONE_ARM64_REG_S26: return ARM64_REG_S26;
    case CAPSTONE_ARM64_REG_S27: return ARM64_REG_S27;
    case CAPSTONE_ARM64_REG_S28: return ARM64_REG_S28;
    case CAPSTONE_ARM64_REG_S29: return ARM64_REG_S29;
    case CAPSTONE_ARM64_REG_S30: return ARM64_REG_S30;
    case CAPSTONE_ARM64_REG_S31: return ARM64_REG_S31;
    case CAPSTONE_ARM64_REG_H0: return ARM64_REG_H0;
    case CAPSTONE_ARM64_REG_H1: return ARM64_REG_H1;
    case CAPSTONE_ARM64_REG_H2: return ARM64_REG_H2;
    case CAPSTONE_ARM64_REG_H3: return ARM64_REG_H3;
    case CAPSTONE_ARM64_REG_H4: return ARM64_REG_H4;
    case CAPSTONE_ARM64_REG_H5: return ARM64_REG_H5;
    case CAPSTONE_ARM64_REG_H6: return ARM64_REG_H6;
    case CAPSTONE_ARM64_REG_H7: return ARM64_REG_H7;
    case CAPSTONE_ARM64_REG_H8: return ARM64_REG_H8;
    case CAPSTONE_ARM64_REG_H9: return ARM64_REG_H9;
    case CAPSTONE_ARM64_REG_H10: return ARM64_REG_H10;
    case CAPSTONE_ARM64_REG_H11: return ARM64_REG_H11;
    case CAPSTONE_ARM64_REG_H12: return ARM64_REG_H12;
    case CAPSTONE_ARM64_REG_H13: return ARM64_REG_H13;
    case CAPSTONE_ARM64_REG_H14: return ARM64_REG_H14;
    case CAPSTONE_ARM64_REG_H15: return ARM64_REG_H15;
    case CAPSTONE_ARM64_REG_H16: return ARM64_REG_H16;
    case CAPSTONE_ARM64_REG_H17: return ARM64_REG_H17;
    case CAPSTONE_ARM64_REG_H18: return ARM64_REG_H18;
    case CAPSTONE_ARM64_REG_H19: return ARM64_REG_H19;
    case CAPSTONE_ARM64_REG_H20: return ARM64_REG_H20;
    case CAPSTONE_ARM64_REG_H21: return ARM64_REG_H21;
    case CAPSTONE_ARM64_REG_H22: return ARM64_REG_H22;
    case CAPSTONE_ARM64_REG_H23: return ARM64_REG_H23;
    case CAPSTONE_ARM64_REG_H24: return ARM64_REG_H24;
    case CAPSTONE_ARM64_REG_H25: return ARM64_REG_H25;
    case CAPSTONE_ARM64_REG_H26: return ARM64_REG_H26;
    case CAPSTONE_ARM64_REG_H27: return ARM64_REG_H27;
    case CAPSTONE_ARM64_REG_H28: return ARM64_REG_H28;
    case CAPSTONE_ARM64_REG_H29: return ARM64_REG_H29;
    case CAPSTONE_ARM64_REG_H30: return ARM64_REG_H30;
    case CAPSTONE_ARM64_REG_H31: return ARM64_REG_H31;
    case CAPSTONE_ARM64_REG_SP: return ARM64_REG_SP;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return ARM64_REG_INVALID;
}

