#include <nxemu-core\Machine\Registers.h>
#include <nxemu-core\SystemGlobals.h>

CRegisters::CRegisters(CPUExecutor * Executor) :
    m_Executor(Executor),
    m_PROGRAM_COUNTER(0),
    m_pstate({ 0 })
{
    memset(m_xregs, 0, sizeof(m_xregs));
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
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return 0;
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
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
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
    case Arm64Opcode::ARM64_CC_NE: return m_pstate.Z == 0;
    case Arm64Opcode::ARM64_CC_LO: return m_pstate.C == 0;
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
        return true;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

