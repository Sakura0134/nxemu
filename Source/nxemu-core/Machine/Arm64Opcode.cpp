#include <nxemu-core\Machine\Arm64Opcode.h>
#include <capstone\Include\capstone.h>
#include <nxemu-core\SystemGlobals.h>

Arm64Opcode::Arm64OpcodeDetail::Arm64OpcodeDetail(uint64_t pc, uint32_t insn) :
    m_pc(pc),
    m_WriteBack(false),
    m_Opc(ARM64_INS_INVALID),
    m_UpdateFlags(false),
    m_cc(ARM64_CC_INVALID)
{
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
    m_Name = results[0].mnemonic;
    m_Param = results[0].op_str;
    m_cc = (Arm64Opcode::arm64_cc)results[0].detail->arm64.cc;
    m_WriteBack = results[0].detail->arm64.writeback;
    m_UpdateFlags = results[0].detail->arm64.update_flags;
    for (uint8_t i = 0, n = results[0].detail->arm64.op_count; i < n; i++)
    {
        cs_arm64_op & src_operand = results[0].detail->arm64.operands[i];
        MCOperand operand;
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
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        m_Operands.push_back(operand);
    }
    cs_free(results, count);
    cs_close(&handle);
}

Arm64Opcode::Arm64Opcode(Arm64OpcodeCache &cache, uint64_t pc, uint32_t insn) :
    m_Details(cache.GetOpcodeDetail(pc, insn))
{
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
    case ARM64_INS_ADRP:
    case ARM64_INS_AND:
    case ARM64_INS_ASR:
    case ARM64_INS_BFI:
    case ARM64_INS_BIC:
    case ARM64_INS_CCMP:
    case ARM64_INS_CINC:
    case ARM64_INS_CLZ:
    case ARM64_INS_CMN:
    case ARM64_INS_CMP:
    case ARM64_INS_CSEL:
    case ARM64_INS_CSET:
    case ARM64_INS_CSINC:
    case ARM64_INS_DUP:
    case ARM64_INS_EOR:
    case ARM64_INS_LDARB:
    case ARM64_INS_LDAXR:
    case ARM64_INS_LDP:
    case ARM64_INS_LDR:
    case ARM64_INS_LDRB:
    case ARM64_INS_LDRSW:
    case ARM64_INS_LDUR:
    case ARM64_INS_LDURB:
    case ARM64_INS_LSL:
    case ARM64_INS_LSR:
    case ARM64_INS_MADD:
    case ARM64_INS_MOV:
    case ARM64_INS_MOVI:
    case ARM64_INS_MOVK:
    case ARM64_INS_MOVN:
    case ARM64_INS_MOVZ:
    case ARM64_INS_MRS:
    case ARM64_INS_MSR:
    case ARM64_INS_MUL:
    case ARM64_INS_MVN:
    case ARM64_INS_NEG:
    case ARM64_INS_ORR:
    case ARM64_INS_RBIT:
    case ARM64_INS_SBFIZ:
    case ARM64_INS_SBFX:
    case ARM64_INS_SMULL:
    case ARM64_INS_STLRB:
    case ARM64_INS_STLXR:
    case ARM64_INS_STP:
    case ARM64_INS_STR:
    case ARM64_INS_STRB:
    case ARM64_INS_STRH:
    case ARM64_INS_STUR:
    case ARM64_INS_STURB:
    case ARM64_INS_STURH:
    case ARM64_INS_SUB:
    case ARM64_INS_SVC:
    case ARM64_INS_SXTW:
    case ARM64_INS_TST:
    case ARM64_INS_UBFIZ:
    case ARM64_INS_UBFX:
    case ARM64_INS_UDIV:
    case ARM64_INS_LDRH:
    case ARM64_INS_MSUB:
    case ARM64_INS_UMADDL:
    case ARM64_INS_UMULH:
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

Arm64OpcodeCache::Arm64OpcodeCache()
{
}

Arm64OpcodeCache::~Arm64OpcodeCache()
{
    CGuard guard(m_CacheCS);

    for (OPCODE_CACHE::const_iterator itr = m_OpcodeCache.begin(); itr != m_OpcodeCache.end(); itr++)
    {
        delete itr->second;
    }
    m_OpcodeCache.clear();
}

Arm64Opcode::Arm64OpcodeDetail * Arm64OpcodeCache::GetOpcodeDetail(uint64_t pc, uint32_t insn)
{
    Arm64Opcode::Arm64OpcodeDetail * Details = NULL;
    OpcodeKey key{ pc,insn };
    {
        CGuard guard(m_CacheCS);
        OPCODE_CACHE::const_iterator itr = m_OpcodeCache.find(key);
        if (itr != m_OpcodeCache.end())
        {
            Details = itr->second;
        }
    }
    if (Details == NULL)
    {
        Details = new Arm64Opcode::Arm64OpcodeDetail(pc, insn);
        {
            CGuard guard(m_CacheCS);
            OPCODE_CACHE::_Pairib res = m_OpcodeCache.insert(OPCODE_CACHE::value_type(key, Details));
            if (!res.second)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
    }
    return Details;
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
    case CAPSTONE_ARM64_REG_SP: return ARM64_REG_SP;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return ARM64_REG_INVALID;
}

