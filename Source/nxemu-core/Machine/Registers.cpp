#include <nxemu-core\Machine\Registers.h>
#include <nxemu-core\SystemGlobals.h>

CRegisters::CRegisters(CPUExecutor * Executor) :
    m_Executor(Executor),
    m_PROGRAM_COUNTER(0),
    m_pstate({ 0 }),
    m_fpcr(0),
    m_tpidrro_el0(0)
{
    memset(m_xregs, 0, sizeof(m_xregs));
    memset(m_vfp_regs, 0, sizeof(m_vfp_regs));
    for (size_t i = 0, n = sizeof(m_wregs) / sizeof(m_wregs[0]); i < n; i++)
    {
        m_wregs[i] = (uint32_t*)&m_xregs[i];
    }
}

uint32_t CRegisters::Get32(Arm64Opcode::arm64_reg reg)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_W0: return *m_wregs[0];
    case Arm64Opcode::ARM64_REG_W1: return *m_wregs[1];
    case Arm64Opcode::ARM64_REG_W2: return *m_wregs[2];
    case Arm64Opcode::ARM64_REG_W3: return *m_wregs[3];
    case Arm64Opcode::ARM64_REG_W4: return *m_wregs[4];
    case Arm64Opcode::ARM64_REG_W5: return *m_wregs[5];
    case Arm64Opcode::ARM64_REG_W6: return *m_wregs[6];
    case Arm64Opcode::ARM64_REG_W7: return *m_wregs[7];
    case Arm64Opcode::ARM64_REG_W8: return *m_wregs[8];
    case Arm64Opcode::ARM64_REG_W9: return *m_wregs[9];
    case Arm64Opcode::ARM64_REG_W10: return *m_wregs[10];
    case Arm64Opcode::ARM64_REG_W11: return *m_wregs[11];
    case Arm64Opcode::ARM64_REG_W12: return *m_wregs[12];
    case Arm64Opcode::ARM64_REG_W13: return *m_wregs[13];
    case Arm64Opcode::ARM64_REG_W14: return *m_wregs[14];
    case Arm64Opcode::ARM64_REG_W15: return *m_wregs[15];
    case Arm64Opcode::ARM64_REG_W16: return *m_wregs[16];
    case Arm64Opcode::ARM64_REG_W17: return *m_wregs[17];
    case Arm64Opcode::ARM64_REG_W18: return *m_wregs[18];
    case Arm64Opcode::ARM64_REG_W19: return *m_wregs[19];
    case Arm64Opcode::ARM64_REG_W20: return *m_wregs[20];
    case Arm64Opcode::ARM64_REG_W21: return *m_wregs[21];
    case Arm64Opcode::ARM64_REG_W22: return *m_wregs[22];
    case Arm64Opcode::ARM64_REG_W23: return *m_wregs[23];
    case Arm64Opcode::ARM64_REG_W24: return *m_wregs[24];
    case Arm64Opcode::ARM64_REG_W25: return *m_wregs[25];
    case Arm64Opcode::ARM64_REG_W26: return *m_wregs[26];
    case Arm64Opcode::ARM64_REG_W27: return *m_wregs[27];
    case Arm64Opcode::ARM64_REG_W28: return *m_wregs[28];
    case Arm64Opcode::ARM64_REG_W29: return *m_wregs[29];
    case Arm64Opcode::ARM64_REG_W30: return *m_wregs[30];
    case Arm64Opcode::ARM64_REG_WZR: return 0;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

uint64_t CRegisters::Get64(Arm64Opcode::arm64_reg reg)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_X0: return m_xregs[0];
    case Arm64Opcode::ARM64_REG_X1: return m_xregs[1];
    case Arm64Opcode::ARM64_REG_X2: return m_xregs[2];
    case Arm64Opcode::ARM64_REG_X3: return m_xregs[3];
    case Arm64Opcode::ARM64_REG_X4: return m_xregs[4];
    case Arm64Opcode::ARM64_REG_X5: return m_xregs[5];
    case Arm64Opcode::ARM64_REG_X6: return m_xregs[6];
    case Arm64Opcode::ARM64_REG_X7: return m_xregs[7];
    case Arm64Opcode::ARM64_REG_X8: return m_xregs[8];
    case Arm64Opcode::ARM64_REG_X9: return m_xregs[9];
    case Arm64Opcode::ARM64_REG_X10: return m_xregs[10];
    case Arm64Opcode::ARM64_REG_X11: return m_xregs[11];
    case Arm64Opcode::ARM64_REG_X12: return m_xregs[12];
    case Arm64Opcode::ARM64_REG_X13: return m_xregs[13];
    case Arm64Opcode::ARM64_REG_X14: return m_xregs[14];
    case Arm64Opcode::ARM64_REG_X15: return m_xregs[15];
    case Arm64Opcode::ARM64_REG_X16: return m_xregs[16];
    case Arm64Opcode::ARM64_REG_X17: return m_xregs[17];
    case Arm64Opcode::ARM64_REG_X18: return m_xregs[18];
    case Arm64Opcode::ARM64_REG_X19: return m_xregs[19];
    case Arm64Opcode::ARM64_REG_X20: return m_xregs[20];
    case Arm64Opcode::ARM64_REG_X21: return m_xregs[21];
    case Arm64Opcode::ARM64_REG_X22: return m_xregs[22];
    case Arm64Opcode::ARM64_REG_X23: return m_xregs[23];
    case Arm64Opcode::ARM64_REG_X24: return m_xregs[24];
    case Arm64Opcode::ARM64_REG_X25: return m_xregs[25];
    case Arm64Opcode::ARM64_REG_X26: return m_xregs[26];
    case Arm64Opcode::ARM64_REG_X27: return m_xregs[27];
    case Arm64Opcode::ARM64_REG_X28: return m_xregs[28];
    case Arm64Opcode::ARM64_REG_X29: return m_xregs[29];
    case Arm64Opcode::ARM64_REG_X30: return m_xregs[30];
    case Arm64Opcode::ARM64_REG_SP: return m_xregs[31];
    case Arm64Opcode::ARM64_REG_XZR: return 0;
    case Arm64Opcode::ARM64_REG_PC: return m_PROGRAM_COUNTER;
    case Arm64Opcode::ARM64_REG_TPIDRRO_EL0: return m_tpidrro_el0;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return 0;
}

void CRegisters::Get128(Arm64Opcode::arm64_reg reg, uint64_t & hiValue, uint64_t & loValue)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_Q0: loValue = m_vfp_regs[1]; hiValue = m_vfp_regs[0]; break;
    case Arm64Opcode::ARM64_REG_Q1: loValue = m_vfp_regs[3]; hiValue = m_vfp_regs[2]; break;
    case Arm64Opcode::ARM64_REG_Q2: loValue = m_vfp_regs[5]; hiValue = m_vfp_regs[4]; break;
    case Arm64Opcode::ARM64_REG_Q3: loValue = m_vfp_regs[7]; hiValue = m_vfp_regs[6]; break;
    case Arm64Opcode::ARM64_REG_Q4: loValue = m_vfp_regs[9]; hiValue = m_vfp_regs[8]; break;
    case Arm64Opcode::ARM64_REG_Q5: loValue = m_vfp_regs[11]; hiValue = m_vfp_regs[10]; break;
    case Arm64Opcode::ARM64_REG_Q6: loValue = m_vfp_regs[13]; hiValue = m_vfp_regs[12]; break;
    case Arm64Opcode::ARM64_REG_Q7: loValue = m_vfp_regs[15]; hiValue = m_vfp_regs[14]; break;
    case Arm64Opcode::ARM64_REG_Q8: loValue = m_vfp_regs[17]; hiValue = m_vfp_regs[16]; break;
    case Arm64Opcode::ARM64_REG_Q9: loValue = m_vfp_regs[19]; hiValue = m_vfp_regs[18]; break;
    case Arm64Opcode::ARM64_REG_Q10: loValue = m_vfp_regs[21]; hiValue = m_vfp_regs[20]; break;
    case Arm64Opcode::ARM64_REG_Q11: loValue = m_vfp_regs[23]; hiValue = m_vfp_regs[22]; break;
    case Arm64Opcode::ARM64_REG_Q12: loValue = m_vfp_regs[25]; hiValue = m_vfp_regs[24]; break;
    case Arm64Opcode::ARM64_REG_Q13: loValue = m_vfp_regs[27]; hiValue = m_vfp_regs[26]; break;
    case Arm64Opcode::ARM64_REG_Q14: loValue = m_vfp_regs[29]; hiValue = m_vfp_regs[28]; break;
    case Arm64Opcode::ARM64_REG_Q15: loValue = m_vfp_regs[31]; hiValue = m_vfp_regs[30]; break;
    case Arm64Opcode::ARM64_REG_Q16: loValue = m_vfp_regs[33]; hiValue = m_vfp_regs[32]; break;
    case Arm64Opcode::ARM64_REG_Q17: loValue = m_vfp_regs[35]; hiValue = m_vfp_regs[34]; break;
    case Arm64Opcode::ARM64_REG_Q18: loValue = m_vfp_regs[37]; hiValue = m_vfp_regs[36]; break;
    case Arm64Opcode::ARM64_REG_Q19: loValue = m_vfp_regs[39]; hiValue = m_vfp_regs[38]; break;
    case Arm64Opcode::ARM64_REG_Q20: loValue = m_vfp_regs[41]; hiValue = m_vfp_regs[40]; break;
    case Arm64Opcode::ARM64_REG_Q21: loValue = m_vfp_regs[43]; hiValue = m_vfp_regs[42]; break;
    case Arm64Opcode::ARM64_REG_Q22: loValue = m_vfp_regs[45]; hiValue = m_vfp_regs[44]; break;
    case Arm64Opcode::ARM64_REG_Q23: loValue = m_vfp_regs[47]; hiValue = m_vfp_regs[46]; break;
    case Arm64Opcode::ARM64_REG_Q24: loValue = m_vfp_regs[49]; hiValue = m_vfp_regs[48]; break;
    case Arm64Opcode::ARM64_REG_Q25: loValue = m_vfp_regs[51]; hiValue = m_vfp_regs[50]; break;
    case Arm64Opcode::ARM64_REG_Q26: loValue = m_vfp_regs[53]; hiValue = m_vfp_regs[52]; break;
    case Arm64Opcode::ARM64_REG_Q27: loValue = m_vfp_regs[55]; hiValue = m_vfp_regs[54]; break;
    case Arm64Opcode::ARM64_REG_Q28: loValue = m_vfp_regs[57]; hiValue = m_vfp_regs[56]; break;
    case Arm64Opcode::ARM64_REG_Q29: loValue = m_vfp_regs[59]; hiValue = m_vfp_regs[58]; break;
    case Arm64Opcode::ARM64_REG_Q30: loValue = m_vfp_regs[61]; hiValue = m_vfp_regs[60]; break;
    case Arm64Opcode::ARM64_REG_Q31: loValue = m_vfp_regs[63]; hiValue = m_vfp_regs[62]; break;
    default:
          g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::Set32(Arm64Opcode::arm64_reg reg, uint32_t value)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_W0: m_xregs[0] = value; break;
    case Arm64Opcode::ARM64_REG_W1: m_xregs[1] = value; break;
    case Arm64Opcode::ARM64_REG_W2: m_xregs[2] = value; break;
    case Arm64Opcode::ARM64_REG_W3: m_xregs[3] = value; break;
    case Arm64Opcode::ARM64_REG_W4: m_xregs[4] = value; break;
    case Arm64Opcode::ARM64_REG_W5: m_xregs[5] = value; break;
    case Arm64Opcode::ARM64_REG_W6: m_xregs[6] = value; break;
    case Arm64Opcode::ARM64_REG_W7: m_xregs[7] = value; break;
    case Arm64Opcode::ARM64_REG_W8: m_xregs[8] = value; break;
    case Arm64Opcode::ARM64_REG_W9: m_xregs[9] = value; break;
    case Arm64Opcode::ARM64_REG_W10: m_xregs[10] = value; break;
    case Arm64Opcode::ARM64_REG_W11: m_xregs[11] = value; break;
    case Arm64Opcode::ARM64_REG_W12: m_xregs[12] = value; break;
    case Arm64Opcode::ARM64_REG_W13: m_xregs[13] = value; break;
    case Arm64Opcode::ARM64_REG_W14: m_xregs[14] = value; break;
    case Arm64Opcode::ARM64_REG_W15: m_xregs[15] = value; break;
    case Arm64Opcode::ARM64_REG_W16: m_xregs[16] = value; break;
    case Arm64Opcode::ARM64_REG_W17: m_xregs[17] = value; break;
    case Arm64Opcode::ARM64_REG_W18: m_xregs[18] = value; break;
    case Arm64Opcode::ARM64_REG_W19: m_xregs[19] = value; break;
    case Arm64Opcode::ARM64_REG_W20: m_xregs[20] = value; break;
    case Arm64Opcode::ARM64_REG_W21: m_xregs[21] = value; break;
    case Arm64Opcode::ARM64_REG_W22: m_xregs[22] = value; break;
    case Arm64Opcode::ARM64_REG_W23: m_xregs[23] = value; break;
    case Arm64Opcode::ARM64_REG_W24: m_xregs[24] = value; break;
    case Arm64Opcode::ARM64_REG_W25: m_xregs[25] = value; break;
    case Arm64Opcode::ARM64_REG_W26: m_xregs[26] = value; break;
    case Arm64Opcode::ARM64_REG_W27: m_xregs[27] = value; break;
    case Arm64Opcode::ARM64_REG_W28: m_xregs[28] = value; break;
    case Arm64Opcode::ARM64_REG_W29: m_xregs[29] = value; break;
    case Arm64Opcode::ARM64_REG_W30: m_xregs[30] = value; break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::Set64(Arm64Opcode::arm64_reg reg, uint64_t value)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_X0: m_xregs[0] = value; break;
    case Arm64Opcode::ARM64_REG_X1: m_xregs[1] = value; break;
    case Arm64Opcode::ARM64_REG_X2: m_xregs[2] = value; break;
    case Arm64Opcode::ARM64_REG_X3: m_xregs[3] = value; break;
    case Arm64Opcode::ARM64_REG_X4: m_xregs[4] = value; break;
    case Arm64Opcode::ARM64_REG_X5: m_xregs[5] = value; break;
    case Arm64Opcode::ARM64_REG_X6: m_xregs[6] = value; break;
    case Arm64Opcode::ARM64_REG_X7: m_xregs[7] = value; break;
    case Arm64Opcode::ARM64_REG_X8: m_xregs[8] = value; break;
    case Arm64Opcode::ARM64_REG_X9: m_xregs[9] = value; break;
    case Arm64Opcode::ARM64_REG_X10: m_xregs[10] = value; break;
    case Arm64Opcode::ARM64_REG_X11: m_xregs[11] = value; break;
    case Arm64Opcode::ARM64_REG_X12: m_xregs[12] = value; break;
    case Arm64Opcode::ARM64_REG_X13: m_xregs[13] = value; break;
    case Arm64Opcode::ARM64_REG_X14: m_xregs[14] = value; break;
    case Arm64Opcode::ARM64_REG_X15: m_xregs[15] = value; break;
    case Arm64Opcode::ARM64_REG_X16: m_xregs[16] = value; break;
    case Arm64Opcode::ARM64_REG_X17: m_xregs[17] = value; break;
    case Arm64Opcode::ARM64_REG_X18: m_xregs[18] = value; break;
    case Arm64Opcode::ARM64_REG_X19: m_xregs[19] = value; break;
    case Arm64Opcode::ARM64_REG_X20: m_xregs[20] = value; break;
    case Arm64Opcode::ARM64_REG_X21: m_xregs[21] = value; break;
    case Arm64Opcode::ARM64_REG_X22: m_xregs[22] = value; break;
    case Arm64Opcode::ARM64_REG_X23: m_xregs[23] = value; break;
    case Arm64Opcode::ARM64_REG_X24: m_xregs[24] = value; break;
    case Arm64Opcode::ARM64_REG_X25: m_xregs[25] = value; break;
    case Arm64Opcode::ARM64_REG_X26: m_xregs[26] = value; break;
    case Arm64Opcode::ARM64_REG_X27: m_xregs[27] = value; break;
    case Arm64Opcode::ARM64_REG_X28: m_xregs[28] = value; break;
    case Arm64Opcode::ARM64_REG_X29: m_xregs[29] = value; break;
    case Arm64Opcode::ARM64_REG_X30: m_xregs[30] = value; break;
    case Arm64Opcode::ARM64_REG_SP: m_xregs[31] = value; break;
    case Arm64Opcode::ARM64_REG_PC: m_PROGRAM_COUNTER = value; break;
    case Arm64Opcode::ARM64_REG_TPIDRRO_EL0: m_tpidrro_el0 = value; break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::Set128(Arm64Opcode::arm64_reg reg, uint64_t hiValue, uint64_t loValue)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_Q0: m_vfp_regs[1] = loValue; m_vfp_regs[0] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q1: m_vfp_regs[3] = loValue; m_vfp_regs[2] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q2: m_vfp_regs[5] = loValue; m_vfp_regs[4] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q3: m_vfp_regs[7] = loValue; m_vfp_regs[6] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q4: m_vfp_regs[9] = loValue; m_vfp_regs[8] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q5: m_vfp_regs[11] = loValue; m_vfp_regs[10] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q6: m_vfp_regs[13] = loValue; m_vfp_regs[12] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q7: m_vfp_regs[15] = loValue; m_vfp_regs[14] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q8: m_vfp_regs[17] = loValue; m_vfp_regs[16] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q9: m_vfp_regs[19] = loValue; m_vfp_regs[18] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q10: m_vfp_regs[21] = loValue; m_vfp_regs[20] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q11: m_vfp_regs[23] = loValue; m_vfp_regs[22] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q12: m_vfp_regs[25] = loValue; m_vfp_regs[24] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q13: m_vfp_regs[27] = loValue; m_vfp_regs[26] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q14: m_vfp_regs[29] = loValue; m_vfp_regs[28] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q15: m_vfp_regs[31] = loValue; m_vfp_regs[30] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q16: m_vfp_regs[33] = loValue; m_vfp_regs[32] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q17: m_vfp_regs[35] = loValue; m_vfp_regs[34] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q18: m_vfp_regs[37] = loValue; m_vfp_regs[36] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q19: m_vfp_regs[39] = loValue; m_vfp_regs[38] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q20: m_vfp_regs[41] = loValue; m_vfp_regs[40] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q21: m_vfp_regs[43] = loValue; m_vfp_regs[42] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q22: m_vfp_regs[45] = loValue; m_vfp_regs[44] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q23: m_vfp_regs[47] = loValue; m_vfp_regs[46] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q24: m_vfp_regs[49] = loValue; m_vfp_regs[48] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q25: m_vfp_regs[51] = loValue; m_vfp_regs[50] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q26: m_vfp_regs[53] = loValue; m_vfp_regs[52] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q27: m_vfp_regs[55] = loValue; m_vfp_regs[54] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q28: m_vfp_regs[57] = loValue; m_vfp_regs[56] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q29: m_vfp_regs[59] = loValue; m_vfp_regs[58] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q30: m_vfp_regs[61] = loValue; m_vfp_regs[60] = hiValue; break;
    case Arm64Opcode::ARM64_REG_Q31: m_vfp_regs[63] = loValue; m_vfp_regs[62] = hiValue; break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::SetFPCR(uint32_t value)
{
    m_fpcr = value;
}

CRegisters::PSTATE CRegisters::GetPstate() const
{
    return m_pstate;
}

void CRegisters::SetConditionFlags(bool n, bool z, bool c, bool v)
{
    m_pstate.N = n;
    m_pstate.Z = z;
    m_pstate.C = c;
    m_pstate.V = v;
}

bool CRegisters::ConditionSet(Arm64Opcode::arm64_cc cc)
{
    switch (cc)
    {
    case Arm64Opcode::ARM64_CC_EQ: return m_pstate.Z != 0;
    case Arm64Opcode::ARM64_CC_NE: return m_pstate.Z == 0;
    case Arm64Opcode::ARM64_CC_HS: return m_pstate.C != 0;
    case Arm64Opcode::ARM64_CC_LO: return m_pstate.C == 0;
    case Arm64Opcode::ARM64_CC_HI: return m_pstate.C != 0 && m_pstate.Z == 0;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool CRegisters::Is32bitReg(Arm64Opcode::arm64_reg reg)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_W0:
    case Arm64Opcode::ARM64_REG_W1:
    case Arm64Opcode::ARM64_REG_W2:
    case Arm64Opcode::ARM64_REG_W3:
    case Arm64Opcode::ARM64_REG_W4:
    case Arm64Opcode::ARM64_REG_W5:
    case Arm64Opcode::ARM64_REG_W6:
    case Arm64Opcode::ARM64_REG_W7:
    case Arm64Opcode::ARM64_REG_W8:
    case Arm64Opcode::ARM64_REG_W9:
    case Arm64Opcode::ARM64_REG_W10:
    case Arm64Opcode::ARM64_REG_W11:
    case Arm64Opcode::ARM64_REG_W12:
    case Arm64Opcode::ARM64_REG_W13:
    case Arm64Opcode::ARM64_REG_W14:
    case Arm64Opcode::ARM64_REG_W15:
    case Arm64Opcode::ARM64_REG_W16:
    case Arm64Opcode::ARM64_REG_W17:
    case Arm64Opcode::ARM64_REG_W18:
    case Arm64Opcode::ARM64_REG_W19:
    case Arm64Opcode::ARM64_REG_W20:
    case Arm64Opcode::ARM64_REG_W21:
    case Arm64Opcode::ARM64_REG_W22:
    case Arm64Opcode::ARM64_REG_W23:
    case Arm64Opcode::ARM64_REG_W24:
    case Arm64Opcode::ARM64_REG_W25:
    case Arm64Opcode::ARM64_REG_W26:
    case Arm64Opcode::ARM64_REG_W27:
    case Arm64Opcode::ARM64_REG_W28:
    case Arm64Opcode::ARM64_REG_W29:
    case Arm64Opcode::ARM64_REG_W30:
    case Arm64Opcode::ARM64_REG_WZR:
        return true;
    case Arm64Opcode::ARM64_REG_X0:
    case Arm64Opcode::ARM64_REG_X1:
    case Arm64Opcode::ARM64_REG_X2:
    case Arm64Opcode::ARM64_REG_X3:
    case Arm64Opcode::ARM64_REG_X4:
    case Arm64Opcode::ARM64_REG_X5:
    case Arm64Opcode::ARM64_REG_X6:
    case Arm64Opcode::ARM64_REG_X7:
    case Arm64Opcode::ARM64_REG_X8:
    case Arm64Opcode::ARM64_REG_X9:
    case Arm64Opcode::ARM64_REG_X10:
    case Arm64Opcode::ARM64_REG_X11:
    case Arm64Opcode::ARM64_REG_X12:
    case Arm64Opcode::ARM64_REG_X13:
    case Arm64Opcode::ARM64_REG_X14:
    case Arm64Opcode::ARM64_REG_X15:
    case Arm64Opcode::ARM64_REG_X16:
    case Arm64Opcode::ARM64_REG_X17:
    case Arm64Opcode::ARM64_REG_X18:
    case Arm64Opcode::ARM64_REG_X19:
    case Arm64Opcode::ARM64_REG_X20:
    case Arm64Opcode::ARM64_REG_X21:
    case Arm64Opcode::ARM64_REG_X22:
    case Arm64Opcode::ARM64_REG_X23:
    case Arm64Opcode::ARM64_REG_X24:
    case Arm64Opcode::ARM64_REG_X25:
    case Arm64Opcode::ARM64_REG_X26:
    case Arm64Opcode::ARM64_REG_X27:
    case Arm64Opcode::ARM64_REG_X28:
    case Arm64Opcode::ARM64_REG_X29:
    case Arm64Opcode::ARM64_REG_X30:
    case Arm64Opcode::ARM64_REG_XZR:
    case Arm64Opcode::ARM64_REG_Q0:
    case Arm64Opcode::ARM64_REG_Q1:
    case Arm64Opcode::ARM64_REG_Q2:
    case Arm64Opcode::ARM64_REG_Q3:
    case Arm64Opcode::ARM64_REG_Q4:
    case Arm64Opcode::ARM64_REG_Q5:
    case Arm64Opcode::ARM64_REG_Q6:
    case Arm64Opcode::ARM64_REG_Q7:
    case Arm64Opcode::ARM64_REG_Q8:
    case Arm64Opcode::ARM64_REG_Q9:
    case Arm64Opcode::ARM64_REG_Q10:
    case Arm64Opcode::ARM64_REG_Q11:
    case Arm64Opcode::ARM64_REG_Q12:
    case Arm64Opcode::ARM64_REG_Q13:
    case Arm64Opcode::ARM64_REG_Q14:
    case Arm64Opcode::ARM64_REG_Q15:
    case Arm64Opcode::ARM64_REG_Q16:
    case Arm64Opcode::ARM64_REG_Q17:
    case Arm64Opcode::ARM64_REG_Q18:
    case Arm64Opcode::ARM64_REG_Q19:
    case Arm64Opcode::ARM64_REG_Q20:
    case Arm64Opcode::ARM64_REG_Q21:
    case Arm64Opcode::ARM64_REG_Q22:
    case Arm64Opcode::ARM64_REG_Q23:
    case Arm64Opcode::ARM64_REG_Q24:
    case Arm64Opcode::ARM64_REG_Q25:
    case Arm64Opcode::ARM64_REG_Q26:
    case Arm64Opcode::ARM64_REG_Q27:
    case Arm64Opcode::ARM64_REG_Q28:
    case Arm64Opcode::ARM64_REG_Q29:
    case Arm64Opcode::ARM64_REG_Q30:
        return false;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool CRegisters::Is64bitReg(Arm64Opcode::arm64_reg reg)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_X0:
    case Arm64Opcode::ARM64_REG_X1:
    case Arm64Opcode::ARM64_REG_X2:
    case Arm64Opcode::ARM64_REG_X3:
    case Arm64Opcode::ARM64_REG_X4:
    case Arm64Opcode::ARM64_REG_X5:
    case Arm64Opcode::ARM64_REG_X6:
    case Arm64Opcode::ARM64_REG_X7:
    case Arm64Opcode::ARM64_REG_X8:
    case Arm64Opcode::ARM64_REG_X9:
    case Arm64Opcode::ARM64_REG_X10:
    case Arm64Opcode::ARM64_REG_X11:
    case Arm64Opcode::ARM64_REG_X12:
    case Arm64Opcode::ARM64_REG_X13:
    case Arm64Opcode::ARM64_REG_X14:
    case Arm64Opcode::ARM64_REG_X15:
    case Arm64Opcode::ARM64_REG_X16:
    case Arm64Opcode::ARM64_REG_X17:
    case Arm64Opcode::ARM64_REG_X18:
    case Arm64Opcode::ARM64_REG_X19:
    case Arm64Opcode::ARM64_REG_X20:
    case Arm64Opcode::ARM64_REG_X21:
    case Arm64Opcode::ARM64_REG_X22:
    case Arm64Opcode::ARM64_REG_X23:
    case Arm64Opcode::ARM64_REG_X24:
    case Arm64Opcode::ARM64_REG_X25:
    case Arm64Opcode::ARM64_REG_X26:
    case Arm64Opcode::ARM64_REG_X27:
    case Arm64Opcode::ARM64_REG_X28:
    case Arm64Opcode::ARM64_REG_X29:
    case Arm64Opcode::ARM64_REG_X30:
    case Arm64Opcode::ARM64_REG_XZR:
    case Arm64Opcode::ARM64_REG_SP:
        return true;
    case Arm64Opcode::ARM64_REG_Q0:
    case Arm64Opcode::ARM64_REG_Q1:
    case Arm64Opcode::ARM64_REG_Q2:
    case Arm64Opcode::ARM64_REG_Q3:
    case Arm64Opcode::ARM64_REG_Q4:
    case Arm64Opcode::ARM64_REG_Q5:
    case Arm64Opcode::ARM64_REG_Q6:
    case Arm64Opcode::ARM64_REG_Q7:
    case Arm64Opcode::ARM64_REG_Q8:
    case Arm64Opcode::ARM64_REG_Q9:
    case Arm64Opcode::ARM64_REG_Q10:
    case Arm64Opcode::ARM64_REG_Q11:
    case Arm64Opcode::ARM64_REG_Q12:
    case Arm64Opcode::ARM64_REG_Q13:
    case Arm64Opcode::ARM64_REG_Q14:
    case Arm64Opcode::ARM64_REG_Q15:
    case Arm64Opcode::ARM64_REG_Q16:
    case Arm64Opcode::ARM64_REG_Q17:
    case Arm64Opcode::ARM64_REG_Q18:
    case Arm64Opcode::ARM64_REG_Q19:
    case Arm64Opcode::ARM64_REG_Q20:
    case Arm64Opcode::ARM64_REG_Q21:
    case Arm64Opcode::ARM64_REG_Q22:
    case Arm64Opcode::ARM64_REG_Q23:
    case Arm64Opcode::ARM64_REG_Q24:
    case Arm64Opcode::ARM64_REG_Q25:
    case Arm64Opcode::ARM64_REG_Q26:
    case Arm64Opcode::ARM64_REG_Q27:
    case Arm64Opcode::ARM64_REG_Q28:
    case Arm64Opcode::ARM64_REG_Q29:
    case Arm64Opcode::ARM64_REG_Q30:
    case Arm64Opcode::ARM64_REG_W0:
    case Arm64Opcode::ARM64_REG_W1:
    case Arm64Opcode::ARM64_REG_W2:
    case Arm64Opcode::ARM64_REG_W3:
    case Arm64Opcode::ARM64_REG_W4:
    case Arm64Opcode::ARM64_REG_W5:
    case Arm64Opcode::ARM64_REG_W6:
    case Arm64Opcode::ARM64_REG_W7:
    case Arm64Opcode::ARM64_REG_W8:
    case Arm64Opcode::ARM64_REG_W9:
    case Arm64Opcode::ARM64_REG_W10:
    case Arm64Opcode::ARM64_REG_W11:
    case Arm64Opcode::ARM64_REG_W12:
    case Arm64Opcode::ARM64_REG_W13:
    case Arm64Opcode::ARM64_REG_W14:
    case Arm64Opcode::ARM64_REG_W15:
    case Arm64Opcode::ARM64_REG_W16:
    case Arm64Opcode::ARM64_REG_W17:
    case Arm64Opcode::ARM64_REG_W18:
    case Arm64Opcode::ARM64_REG_W19:
    case Arm64Opcode::ARM64_REG_W20:
    case Arm64Opcode::ARM64_REG_W21:
    case Arm64Opcode::ARM64_REG_W22:
    case Arm64Opcode::ARM64_REG_W23:
    case Arm64Opcode::ARM64_REG_W24:
    case Arm64Opcode::ARM64_REG_W25:
    case Arm64Opcode::ARM64_REG_W26:
    case Arm64Opcode::ARM64_REG_W27:
    case Arm64Opcode::ARM64_REG_W28:
    case Arm64Opcode::ARM64_REG_W29:
    case Arm64Opcode::ARM64_REG_W30:
    case Arm64Opcode::ARM64_REG_WZR:
        return false;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool CRegisters::Is128bitReg(Arm64Opcode::arm64_reg reg)
{
    switch (reg)
    {
    case Arm64Opcode::ARM64_REG_Q0:
    case Arm64Opcode::ARM64_REG_Q1:
    case Arm64Opcode::ARM64_REG_Q2:
    case Arm64Opcode::ARM64_REG_Q3:
    case Arm64Opcode::ARM64_REG_Q4:
    case Arm64Opcode::ARM64_REG_Q5:
    case Arm64Opcode::ARM64_REG_Q6:
    case Arm64Opcode::ARM64_REG_Q7:
    case Arm64Opcode::ARM64_REG_Q8:
    case Arm64Opcode::ARM64_REG_Q9:
    case Arm64Opcode::ARM64_REG_Q10:
    case Arm64Opcode::ARM64_REG_Q11:
    case Arm64Opcode::ARM64_REG_Q12:
    case Arm64Opcode::ARM64_REG_Q13:
    case Arm64Opcode::ARM64_REG_Q14:
    case Arm64Opcode::ARM64_REG_Q15:
    case Arm64Opcode::ARM64_REG_Q16:
    case Arm64Opcode::ARM64_REG_Q17:
    case Arm64Opcode::ARM64_REG_Q18:
    case Arm64Opcode::ARM64_REG_Q19:
    case Arm64Opcode::ARM64_REG_Q20:
    case Arm64Opcode::ARM64_REG_Q21:
    case Arm64Opcode::ARM64_REG_Q22:
    case Arm64Opcode::ARM64_REG_Q23:
    case Arm64Opcode::ARM64_REG_Q24:
    case Arm64Opcode::ARM64_REG_Q25:
    case Arm64Opcode::ARM64_REG_Q26:
    case Arm64Opcode::ARM64_REG_Q27:
    case Arm64Opcode::ARM64_REG_Q28:
    case Arm64Opcode::ARM64_REG_Q29:
    case Arm64Opcode::ARM64_REG_Q30:
        return true;
    case Arm64Opcode::ARM64_REG_X0:
    case Arm64Opcode::ARM64_REG_X1:
    case Arm64Opcode::ARM64_REG_X2:
    case Arm64Opcode::ARM64_REG_X3:
    case Arm64Opcode::ARM64_REG_X4:
    case Arm64Opcode::ARM64_REG_X5:
    case Arm64Opcode::ARM64_REG_X6:
    case Arm64Opcode::ARM64_REG_X7:
    case Arm64Opcode::ARM64_REG_X8:
    case Arm64Opcode::ARM64_REG_X9:
    case Arm64Opcode::ARM64_REG_X10:
    case Arm64Opcode::ARM64_REG_X11:
    case Arm64Opcode::ARM64_REG_X12:
    case Arm64Opcode::ARM64_REG_X13:
    case Arm64Opcode::ARM64_REG_X14:
    case Arm64Opcode::ARM64_REG_X15:
    case Arm64Opcode::ARM64_REG_X16:
    case Arm64Opcode::ARM64_REG_X17:
    case Arm64Opcode::ARM64_REG_X18:
    case Arm64Opcode::ARM64_REG_X19:
    case Arm64Opcode::ARM64_REG_X20:
    case Arm64Opcode::ARM64_REG_X21:
    case Arm64Opcode::ARM64_REG_X22:
    case Arm64Opcode::ARM64_REG_X23:
    case Arm64Opcode::ARM64_REG_X24:
    case Arm64Opcode::ARM64_REG_X25:
    case Arm64Opcode::ARM64_REG_X26:
    case Arm64Opcode::ARM64_REG_X27:
    case Arm64Opcode::ARM64_REG_X28:
    case Arm64Opcode::ARM64_REG_X29:
    case Arm64Opcode::ARM64_REG_X30:
    case Arm64Opcode::ARM64_REG_XZR:
        return false;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}
