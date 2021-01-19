#include <nxemu-core\Machine\Registers.h>
#include <nxemu-core\SystemGlobals.h>

CRegisters::CRegisters() :
    m_PROGRAM_COUNTER(0),
    m_pstate({ 0 }),
    m_fpcr(0),
    m_fpsr(0),
    m_tpidr_el0(0),
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
    if (reg >= Arm64Opcode::ARM64_REG_W0 && reg <= Arm64Opcode::ARM64_REG_W30)
    {
        return *m_wregs[reg - Arm64Opcode::ARM64_REG_W0];
    }
    if (reg == Arm64Opcode::ARM64_REG_WZR)
    {
        return 0;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

float32_t CRegisters::Get32Float(Arm64Opcode::arm64_reg reg)
{
    float32_t value = { 0 };

    if (reg >= Arm64Opcode::ARM64_REG_S0 && reg <= Arm64Opcode::ARM64_REG_S31)
    {
        value.v = (uint32_t)m_vfp_regs[(reg - Arm64Opcode::ARM64_REG_S0) << 1];
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return value;
}

uint64_t CRegisters::Get64(Arm64Opcode::arm64_reg reg)
{
    if (reg >= Arm64Opcode::ARM64_REG_X0 && reg <= Arm64Opcode::ARM64_REG_X30)
    {
        return m_xregs[reg - Arm64Opcode::ARM64_REG_X0];
    }
    if (reg == Arm64Opcode::ARM64_REG_SP)
    {
        return m_xregs[31];
    }
    if (reg == Arm64Opcode::ARM64_REG_XZR)
    {
        return 0;
    }
    if (reg == Arm64Opcode::ARM64_REG_PC)
    {
        return m_PROGRAM_COUNTER;
    }
    if (reg == Arm64Opcode::ARM64_REG_TPIDR_EL0)
    {
        return m_tpidr_el0;
    }
    if (reg == Arm64Opcode::ARM64_REG_TPIDRRO_EL0)
    {
        return m_tpidrro_el0;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

float64_t CRegisters::Get64Float(Arm64Opcode::arm64_reg reg)
{
    float64_t value = { 0 };
    if (reg >= Arm64Opcode::ARM64_REG_D0 && reg <= Arm64Opcode::ARM64_REG_D31)
    {
        value.v = m_vfp_regs[(reg - Arm64Opcode::ARM64_REG_D0) << 1];
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return value;
}

uint64_t CRegisters::Get64Vector(Arm64Opcode::arm64_reg reg, int64_t VectorIndex, Arm64Opcode::arm64_vess Vess, Arm64Opcode::arm64_vas Vas)
{
    if (VectorIndex == -1 && Vess == Arm64Opcode::ARM64_VESS_INVALID && Vas == Arm64Opcode::ARM64_VAS_8B)
    {
        if (reg >= Arm64Opcode::ARM64_REG_V0 && reg <= Arm64Opcode::ARM64_REG_V31)
        {
            return m_vfp_regs[(reg - Arm64Opcode::ARM64_REG_V0) * 2];
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return 0;
}

void CRegisters::Get128(Arm64Opcode::arm64_reg reg, uint64_t & hiValue, uint64_t & loValue)
{
    if (reg >= Arm64Opcode::ARM64_REG_Q0 && reg <= Arm64Opcode::ARM64_REG_Q31)
    {
        int Index = (reg - Arm64Opcode::ARM64_REG_Q0) << 1;
        hiValue = m_vfp_regs[Index];
        loValue = m_vfp_regs[Index + 1];
    }
    else if (reg >= Arm64Opcode::ARM64_REG_V0 && reg <= Arm64Opcode::ARM64_REG_V31)
    {
        int Index = (reg - Arm64Opcode::ARM64_REG_V0) << 1;
        hiValue = m_vfp_regs[Index];
        loValue = m_vfp_regs[Index + 1];
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::Set32(Arm64Opcode::arm64_reg reg, uint32_t value)
{
    if (reg >= Arm64Opcode::ARM64_REG_W0 && reg <= Arm64Opcode::ARM64_REG_W30)
    {
        m_xregs[reg - Arm64Opcode::ARM64_REG_W0] = value;
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::Set32Float(Arm64Opcode::arm64_reg reg, float32_t value)
{
    if (reg >= Arm64Opcode::ARM64_REG_S0 && reg <= Arm64Opcode::ARM64_REG_S31)
    {
        int Index = (reg - Arm64Opcode::ARM64_REG_S0) << 1;
        m_vfp_regs[Index] = value.v;
        m_vfp_regs[Index + 1] = 0;
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::Set64(Arm64Opcode::arm64_reg reg, uint64_t value)
{
    if (reg >= Arm64Opcode::ARM64_REG_X0 && reg <= Arm64Opcode::ARM64_REG_X30)
    {
        m_xregs[reg - Arm64Opcode::ARM64_REG_X0] = value;
    }
    else if (reg == Arm64Opcode::ARM64_REG_SP)
    {
        m_xregs[31] = value;
    }
    else if (reg == Arm64Opcode::ARM64_REG_PC)
    {
        m_PROGRAM_COUNTER = value;
    }
    else if (reg == Arm64Opcode::ARM64_REG_TPIDRRO_EL0)
    {
        m_tpidrro_el0 = value;
    }
    else if (reg == Arm64Opcode::ARM64_REG_TPIDR_EL0)
    {
        m_tpidr_el0 = value;
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::Set64Float(Arm64Opcode::arm64_reg reg, float64_t value)
{
    if (reg >= Arm64Opcode::ARM64_REG_D0 && reg <= Arm64Opcode::ARM64_REG_D31)
    {
        int Index = (reg - Arm64Opcode::ARM64_REG_D0) << 1;
        m_vfp_regs[Index] = value.v;
        m_vfp_regs[Index + 1] = 0;
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::Set128(Arm64Opcode::arm64_reg reg, uint64_t hiValue, uint64_t loValue)
{
    if (reg >= Arm64Opcode::ARM64_REG_Q0 && reg <= Arm64Opcode::ARM64_REG_Q31)
    {
        int Index = (reg - Arm64Opcode::ARM64_REG_Q0) << 1;
        m_vfp_regs[Index] = hiValue;
        m_vfp_regs[Index + 1] = loValue;
    }
    else if (reg >= Arm64Opcode::ARM64_REG_V0 && reg <= Arm64Opcode::ARM64_REG_V31)
    {
        int Index = (reg - Arm64Opcode::ARM64_REG_V0) << 1;
        m_vfp_regs[Index] = hiValue;
        m_vfp_regs[Index + 1] = loValue;
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::Set64Vector(Arm64Opcode::arm64_reg reg, int64_t VectorIndex, Arm64Opcode::arm64_vess Vess, Arm64Opcode::arm64_vas Vas, uint64_t value)
{
    switch (Vess)
    {
    case Arm64Opcode::ARM64_VESS_INVALID:
        if (VectorIndex == -1 && Vas == Arm64Opcode::ARM64_VAS_8B && reg >= Arm64Opcode::ARM64_REG_V0 && reg <= Arm64Opcode::ARM64_REG_V31)
        {
            m_vfp_regs[(reg - Arm64Opcode::ARM64_REG_V0) * 2] = value;
            m_vfp_regs[((reg - Arm64Opcode::ARM64_REG_V0) * 2) + 1] = 0;
        }
        else if (VectorIndex == -1 && Vas == Arm64Opcode::ARM64_VAS_2D && reg >= Arm64Opcode::ARM64_REG_V0 && reg <= Arm64Opcode::ARM64_REG_V31)
        {
            m_vfp_regs[(reg - Arm64Opcode::ARM64_REG_V0) * 2] = value;
            m_vfp_regs[((reg - Arm64Opcode::ARM64_REG_V0) * 2) + 1] = value;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        break;
    case Arm64Opcode::ARM64_VESS_D:
        if (Vas == Arm64Opcode::ARM64_VAS_INVALID && reg >= Arm64Opcode::ARM64_REG_V0 && reg <= Arm64Opcode::ARM64_REG_V31)
        {
            if (VectorIndex > 1)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            m_vfp_regs[(reg - Arm64Opcode::ARM64_REG_V0) * 2 + VectorIndex] = value;
        }
        else if (Vas == Arm64Opcode::ARM64_VAS_2D && reg >= Arm64Opcode::ARM64_REG_V0 && reg <= Arm64Opcode::ARM64_REG_V31)
        {
            if (VectorIndex > 0)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            m_vfp_regs[(reg - Arm64Opcode::ARM64_REG_V0) * 2] = value;
            m_vfp_regs[((reg - Arm64Opcode::ARM64_REG_V0) * 2) + 1] = value;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CRegisters::SetFPCR(uint32_t value)
{
    m_fpcr = value;
}

void CRegisters::SetFPSR(uint32_t value)
{
    m_fpsr = value;
}

PSTATE CRegisters::GetPstate() const
{
    return m_pstate;
}

uint32_t CRegisters::GetFPCR() const
{
    return m_fpcr;
}

uint32_t CRegisters::GetFPSR() const
{
    return m_fpsr;
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
    case Arm64Opcode::ARM64_CC_VC: return m_pstate.V == 0;
    case Arm64Opcode::ARM64_CC_HI: return m_pstate.C != 0 && m_pstate.Z == 0;
    case Arm64Opcode::ARM64_CC_LS: return m_pstate.C == 0 || m_pstate.Z != 0;
    case Arm64Opcode::ARM64_CC_GE: return m_pstate.N == m_pstate.V;
    case Arm64Opcode::ARM64_CC_LT: return m_pstate.N != m_pstate.V;
    case Arm64Opcode::ARM64_CC_GT: return m_pstate.Z == 0 && m_pstate.N == m_pstate.V;
    case Arm64Opcode::ARM64_CC_LE: return m_pstate.Z != 0 || (m_pstate.N != m_pstate.V);
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}