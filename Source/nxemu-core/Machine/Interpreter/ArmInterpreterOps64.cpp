#include <nxemu-core\Machine\Interpreter\ArmInterpreterOps64.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\Maths.h>

void Arm64Op::Add(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) + op.Operand(2).ImmVal);
        }
        else if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(2).Reg))
        {
            if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) + Reg.Get64(op.Operand(2).Reg));
            }
            else if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) + (Reg.Get64(op.Operand(2).Reg) << op.Operand(2).shift.value));
            }
            else if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSR && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) + (Reg.Get64(op.Operand(2).Reg) >> op.Operand(2).shift.value));
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(2).Reg))
        {
            if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_UXTW)
            {
                Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) + (uint32_t)(Reg.Get32(op.Operand(2).Reg)));
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
    }
    else if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(2).Reg))
        {
            if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) + Reg.Get32(op.Operand(2).Reg));
            }
            else if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_UXTB)
            {
                Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) + (uint8_t)(Reg.Get32(op.Operand(2).Reg)));
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set32(op.Operand(0).Reg, (uint32_t)(Reg.Get32(op.Operand(1).Reg) + op.Operand(2).ImmVal));
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
}

void Arm64Op::Adrp(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        CRegisters & Reg = core.Reg();
        Reg.Set64(op.Operand(0).Reg, op.Operand(1).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::And(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) & op.Operand(2).ImmVal);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) & op.Operand(2).ImmVal);
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
}

void Arm64Op::B(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 1 && op.Operand(0).type == Arm64Opcode::ARM64_OP_IMM)
    {
        CRegisters & Reg = core.Reg();
        Reg.Set64(Arm64Opcode::ARM64_REG_PC, op.Operand(0).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Bfi(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 4 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && op.Operand(3).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
        {
            int64_t lsb = op.Operand(2).ImmVal;
            int64_t width = op.Operand(3).ImmVal;

            if (lsb < 0 || lsb > 63)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            if (width < 1 || width > 64)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }

            uint64_t signbit = ((uint64_t)1 << width) - 1;
            uint64_t OrigialValue = Reg.Get64(op.Operand(0).Reg) & (~(signbit << lsb));
            Reg.Set64(op.Operand(0).Reg, OrigialValue | (Reg.Get64(op.Operand(1).Reg) & signbit) << lsb);
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
}

void Arm64Op::Bic(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(2).Reg) && op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
        {
            Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) & ~(Reg.Get64(op.Operand(2).Reg)));
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
}

void Arm64Op::Bl(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 1 && op.Operand(0).type == Arm64Opcode::ARM64_OP_IMM)
    {
        CRegisters & Reg = core.Reg();
        Reg.Set64(Arm64Opcode::ARM64_REG_LR, Reg.Get64(Arm64Opcode::ARM64_REG_PC) + 4);
        Reg.Set64(Arm64Opcode::ARM64_REG_PC, op.Operand(0).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Br(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 1 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        CRegisters & Reg = core.Reg();
        Reg.Set64(Arm64Opcode::ARM64_REG_PC, Reg.Get64(op.Operand(0).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Cbnz(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (CRegisters::Is64bitReg(op.Operand(0).Reg))
        {
            if (Reg.Get64(op.Operand(0).Reg) != 0)
            {
                Reg.Set64(Arm64Opcode::ARM64_REG_PC, op.Operand(1).ImmVal);
                core.Jumped();
            }
        }
        else if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            if (Reg.Get32(op.Operand(0).Reg) != 0)
            {
                Reg.Set64(Arm64Opcode::ARM64_REG_PC, op.Operand(1).ImmVal);
                core.Jumped();
            }
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
}

void Arm64Op::Cbz(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (Reg.Get64(op.Operand(0).Reg) == 0)
        {
            Reg.Set64(Arm64Opcode::ARM64_REG_PC, op.Operand(1).ImmVal);
            core.Jumped();
        }
    }
    else if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (Reg.Get32(op.Operand(0).Reg) == 0)
        {
            Reg.Set64(Arm64Opcode::ARM64_REG_PC, op.Operand(1).ImmVal);
            core.Jumped();
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ccmp(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg))
    {
        int64_t ImmVal;
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(2).Reg))
        {
            ImmVal = Reg.Get32(op.Operand(2).Reg);
        }
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            ImmVal = op.Operand(2).ImmVal;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }

        bool z = (ImmVal & 4) != 0;
        bool n = (ImmVal & 8) != 0;
        bool c = (ImmVal & 2) != 0;
        bool v = (ImmVal & 1) != 0;
        if (Reg.ConditionSet(op.cc()))
        {
            uint32_t a = Reg.Get32(op.Operand(0).Reg);
            uint32_t b;
            if (op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(1).Reg))
            {
               b = Reg.Get32(op.Operand(1).Reg);
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            uint32_t result = a - b;
            n = (result & 0x80000000) != 0;
            z = result == 0;
            c = a >= b; //if the result of a subtraction is positive or zero
            v = ((((a ^ b) & (a ^ result)) >> 20) & 0x80000000) != 0;
        }
        else
        {
            if ((ImmVal & 0xB) != 0)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        Reg.SetConditionFlags(n, z, c, v);
    }
    else if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg))
    {
        int64_t ImmVal;
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            ImmVal = op.Operand(2).ImmVal;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        bool z = (ImmVal & 4) != 0;
        bool n = (ImmVal & 8) != 0;
        bool c = (ImmVal & 2) != 0;
        bool v = (ImmVal & 1) != 0;
        if (Reg.ConditionSet(op.cc()))
        {
            uint64_t a = Reg.Get64(op.Operand(0).Reg), b;
            if (op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
            {
                b = op.Operand(1).ImmVal;
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            uint64_t result = a - b;
            n = (result & 0x80000000) != 0;
            z = result == 0;
            c = a >= b; //if the result of a subtraction is positive or zero
            v = ((((a ^ b) & (a ^ result)) >> 20) & 0x80000000) != 0;
        }
        else
        {
            n = (ImmVal & 8) != 0;
            z = (ImmVal & 4) != 0;
            c = (ImmVal & 2) != 0;
            v = (ImmVal & 1) != 0;
        }
        Reg.SetConditionFlags(n, z, c, v);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Clz(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        Reg.Set64(op.Operand(0).Reg,clz64(Reg.Get64(op.Operand(1).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Cmn(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    uint64_t a, b;
    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && CRegisters::Is64bitReg(op.Operand(0).Reg))
    {
        if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            a = Reg.Get64(op.Operand(0).Reg);
            b = op.Operand(1).ImmVal;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    uint64_t result = a + b;
    uint32_t result_lo = (uint32_t)(result);
    uint32_t result_hi = (uint32_t)((result >> 32) & 0xFFFFFFFF);

    bool n = (result_lo & 0x80000000) != 0;
    bool z = result_lo == 0;
    bool c = (result_hi & 1) != 0;
    bool v = ((result_lo ^ a) & (~(a ^ b)) & 0x80000000) != 0;
    Reg.SetConditionFlags(n, z, c, v);
}

void Arm64Op::Cmp(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    uint64_t a, b;
    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        a = Reg.Get64(op.Operand(0).Reg);
        b = op.Operand(1).ImmVal;
    }
    else if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        a = (int64_t)((int32_t)Reg.Get32(op.Operand(0).Reg));
        b = op.Operand(1).ImmVal;
    }
    else if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        a = Reg.Get64(op.Operand(0).Reg);
        b = Reg.Get64(op.Operand(1).Reg);
    }
    else if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(1).Reg))
    {
        a = (int64_t)((int32_t)Reg.Get32(op.Operand(0).Reg));
        b = (int64_t)((int32_t)Reg.Get32(op.Operand(1).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    if (op.Operand(1).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
    {
        if (op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_LSR)
        {
            b >>= op.Operand(1).shift.value;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }

    uint64_t result = a - b;
    bool n = (result & 0x8000000000000000) != 0;
    bool z = result == 0;
    bool c = a >= b; //if the result of a subtraction is positive or zero
    bool v = ((((a ^ b) & (a ^ result)) >> 20) & 0x80000000) != 0;
    Reg.SetConditionFlags(n, z, c, v);
}

void Arm64Op::Csel(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg) && CRegisters::Is32bitReg(op.Operand(2).Reg))
        {
            Reg.Set32(op.Operand(0).Reg, Reg.ConditionSet(op.cc()) ? Reg.Get32(op.Operand(1).Reg) : Reg.Get32(op.Operand(2).Reg));
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
}

void Arm64Op::Csinc(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
    {
        Reg.Set32(op.Operand(0).Reg, Reg.ConditionSet(op.cc()) ? Reg.Get32(op.Operand(1).Reg) : (Reg.Get32(op.Operand(2).Reg) + 1));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Eor(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
  
    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(2).Reg))
        {
            if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
            {
                Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) ^ Reg.Get64(op.Operand(2).Reg));
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
    }
    else if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(2).Reg))
        {
            if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
            {
                Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) ^ Reg.Get32(op.Operand(2).Reg));
            }
            else if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL)
            {
                Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) ^ (Reg.Get32(op.Operand(2).Reg) << op.Operand(2).shift.value));
            }
            else if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSR)
            {
                Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) ^ (Reg.Get32(op.Operand(2).Reg) >> op.Operand(2).shift.value));
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
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Cset(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 1 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            Reg.Set32(op.Operand(0).Reg, Reg.ConditionSet(op.cc()) ? 1 : 0);
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
}

void Arm64Op::Dup(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG)
    {
        // this is wrong, it is intilizing the vector to 0 in boot.
        //When vector registers are being used this code needs to be fixed
        if (Reg.Get64(op.Operand(1).Reg) != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldaxr(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        MemoryManagement & MMU = core.MMU();
        CRegisters & Reg = core.Reg();

        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (op.Operand(1).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        uint64_t load_addr = Reg.Get64(op.Operand(1).mem.base) + op.Operand(1).mem.disp;
        if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            uint32_t value;
            if (!MMU.Read32(load_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(op.Operand(0).Reg, value);
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
}

void Arm64Op::Ldp(CPUExecutor & core, const Arm64Opcode &op)
{
    MemoryManagement & MMU = core.MMU();
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM ||
        op.Operands() == 4 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM && op.Operand(3).type == Arm64Opcode::ARM64_OP_IMM)
    {
        uint64_t load_addr = Reg.Get64(op.Operand(2).mem.base) + op.Operand(2).mem.disp;

        if (CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
        {
            uint64_t value;
            if (!MMU.Read64(load_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(op.Operand(0).Reg, value);

            if (!MMU.Read64(load_addr + 8, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(op.Operand(1).Reg, value);
        }
        else if (CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
        {
            uint32_t value;
            if (!MMU.Read32(load_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(op.Operand(0).Reg, value);

            if (!MMU.Read32(load_addr + 4, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(op.Operand(1).Reg, value);
        }
        else if (CRegisters::Is128bitReg(op.Operand(0).Reg) && CRegisters::Is128bitReg(op.Operand(1).Reg))
        {
            uint64_t valueHi, valueLo;
            if (!MMU.Read64(load_addr + 0x00, valueHi)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            if (!MMU.Read64(load_addr + 0x08, valueLo)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set128(op.Operand(0).Reg, valueHi, valueLo);

            if (!MMU.Read64(load_addr + 0x10, valueHi)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            if (!MMU.Read64(load_addr + 0x18, valueLo)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set128(op.Operand(1).Reg, valueHi, valueLo);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (op.WriteBack() || op.Operands() == 4)
        {
            Reg.Set64(op.Operand(2).mem.base, op.Operands() == 4 ? load_addr + op.Operand(3).ImmVal : load_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldr(CPUExecutor & core, const Arm64Opcode &op)
{
    if ((op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        CRegisters & Reg = core.Reg();
        uint64_t index = 0;
        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            if (CRegisters::Is64bitReg(op.Operand(1).mem.index))
            {
                index = Reg.Get64(op.Operand(1).mem.index);
            }
            else if (CRegisters::Is32bitReg(op.Operand(1).mem.index))
            {
                index = Reg.Get32(op.Operand(1).mem.index);
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }

        if (op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_LSL)
        {
            index <<= op.Operand(1).shift.value;
        }
        else if (op.Operand(1).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        MemoryManagement & MMU = core.MMU();
        uint64_t load_addr;
        if (CRegisters::Is64bitReg(op.Operand(1).mem.base))
        {
            load_addr = Reg.Get64(op.Operand(1).mem.base) + op.Operand(1).mem.disp + index;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if (CRegisters::Is64bitReg(op.Operand(0).Reg))
        {
            uint64_t value;
            if (!MMU.Read64(load_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(op.Operand(0).Reg, value);
        }
        else if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            uint32_t value;
            if (!MMU.Read32(load_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (op.WriteBack() || op.Operands() == 3)
        {
            if (CRegisters::Is64bitReg(op.Operand(1).mem.base))
            {
                Reg.Set64(op.Operand(1).mem.base, op.Operands() == 3 ? load_addr + op.Operand(2).ImmVal : load_addr);
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldrb(CPUExecutor & core, const Arm64Opcode &op)
{
    MemoryManagement & MMU = core.MMU();

    if ((op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        CRegisters & Reg = core.Reg();
        uint64_t index = 0;
        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (op.Operand(1).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        uint64_t load_addr = Reg.Get64(op.Operand(1).mem.base) + op.Operand(1).mem.disp + index;
        if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            uint8_t value;
            if (!MMU.Read8(load_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (op.WriteBack() || op.Operands() == 3)
        {
            Reg.Set64(op.Operand(1).mem.base, op.Operands() == 3 ? load_addr + op.Operand(2).ImmVal : load_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldrh(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    MemoryManagement & MMU = core.MMU();

    if ((op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM))
    {
        uint64_t index = 0;
        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (op.Operand(1).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        uint64_t load_addr = Reg.Get64(op.Operand(1).mem.base) + op.Operand(1).mem.disp + index;
        if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            uint16_t value;
            if (!MMU.Read16(load_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldrsw(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    MemoryManagement & MMU = core.MMU();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = 0;
        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            index = Reg.Get64(op.Operand(1).mem.index);
        }

        if (op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_LSL)
        {
            index <<= op.Operand(1).shift.value;
        }
        else if (op.Operand(1).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        uint64_t load_addr = Reg.Get64(op.Operand(1).mem.base) + op.Operand(1).mem.disp + index;

        if (CRegisters::Is64bitReg(op.Operand(0).Reg))
        {
            uint32_t value;
            if (!MMU.Read32(load_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(op.Operand(0).Reg, (uint64_t)((int64_t)((int32_t)value)));
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
}

void Arm64Op::Ldur(CPUExecutor & core, const Arm64Opcode &op)
{
    MemoryManagement & MMU = core.MMU();
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID) { g_Notify->BreakPoint(__FILE__, __LINE__); }
        if (op.Operand(1).shift.type != Arm64Opcode::ARM64_SFT_INVALID) { g_Notify->BreakPoint(__FILE__, __LINE__); }

        uint64_t load_addr = Reg.Get64(op.Operand(1).mem.base) + op.Operand(1).mem.disp;
        if (CRegisters::Is64bitReg(op.Operand(0).Reg))
        {
            uint64_t value;
            if (!MMU.Read64(load_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(op.Operand(0).Reg, value);
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
}

void Arm64Op::Lsl(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg) && op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) << op.Operand(2).ImmVal);
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
}

void Arm64Op::Lsr(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg) && CRegisters::Is64bitReg(op.Operand(2).Reg))
        {
            Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) >> Reg.Get64(op.Operand(2).Reg));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
        {
            Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) >> op.Operand(2).ImmVal);
        }
        else if (CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
        {
            Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) >> op.Operand(2).ImmVal);
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
}

void Arm64Op::Madd(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 4 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && op.Operand(3).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg) && CRegisters::Is64bitReg(op.Operand(2).Reg) && CRegisters::Is64bitReg(op.Operand(3).Reg))
    {
        Reg.Set64(op.Operand(0).Reg, (Reg.Get64(op.Operand(1).Reg) * Reg.Get64(op.Operand(2).Reg) + Reg.Get64(op.Operand(3).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Mov(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
        {
            if (op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
            {
                Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg));
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg) )
        {
            if (op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
            {
                Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg));
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
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sub(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        uint64_t a = 0, b = 0;
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            a = Reg.Get64(op.Operand(1).Reg);
            b = op.Operand(2).ImmVal;
        }
        else if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(2).Reg))
        {
            a = Reg.Get64(op.Operand(1).Reg);
            if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get64(op.Operand(2).Reg);
            }
            else if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get64(op.Operand(2).Reg) << op.Operand(2).shift.value;
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

        uint64_t result = a - b;
        Reg.Set64(op.Operand(0).Reg, result);
        if (op.UpdateFlags())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
    {
        uint32_t a = 0, b = 0;
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            a = Reg.Get32(op.Operand(1).Reg);
            b = (uint32_t)(op.Operand(2).ImmVal);
        }
        else if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(2).Reg))
        {
            a = Reg.Get32(op.Operand(1).Reg);
            if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get32(op.Operand(2).Reg);
            }
            else if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_UXTB)
            {
                b = (uint8_t)Reg.Get32(op.Operand(2).Reg);
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
        uint32_t result = a - b;
        Reg.Set32(op.Operand(0).Reg, result);
        if (op.UpdateFlags())
        {
            bool n = (result & 0x80000000) != 0;
            bool z = result == 0;
            bool c = a >= b; //if the result of a subtraction is positive or zero
            bool v = ((((a ^ b) & (a ^ result)) >> 20) & 0x80000000) != 0;

            Reg.SetConditionFlags(n, z, c, v);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Movk(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(0).Reg))
    {
        if (op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
        {
            Reg.Set64(op.Operand(0).Reg, (Reg.Get64(op.Operand(0).Reg) & ~0xFFFF) | (uint16_t)(op.Operand(1).ImmVal & 0xFFFF));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(0).Reg))
    {
        if (op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
        {
            Reg.Set32(op.Operand(0).Reg, (Reg.Get32(op.Operand(0).Reg) & ~0xFFFF) | (uint16_t)(op.Operand(1).ImmVal & 0xFFFF));
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
}

void Arm64Op::Movn(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(0).Reg))
    {
        if (op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(op.Operand(0).Reg, ~op.Operand(1).ImmVal);
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
}

void Arm64Op::Movz(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (CRegisters::Is64bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(op.Operand(0).Reg, op.Operand(1).ImmVal);
        }
        else if (CRegisters::Is32bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set32(op.Operand(0).Reg, (uint32_t)op.Operand(1).ImmVal);
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
}

void Arm64Op::Mrs(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG_MRS)
    {
        switch (op.Operand(1).SysReg)
        {
        case Arm64Opcode::A64SysReg_TPIDRRO_EL0:
            Reg.Set64(op.Operand(0).Reg, Reg.Get64(Arm64Opcode::ARM64_REG_TPIDRRO_EL0));
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Msr(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG_MSR && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        switch (op.Operand(0).SysReg)
        {
        case Arm64Opcode::A64SysReg_FPCR:
            Reg.SetFPCR((uint32_t)Reg.Get64(op.Operand(1).Reg));
            break;
        case Arm64Opcode::A64SysReg_FPSR:
            Reg.SetFPSR((uint32_t)Reg.Get64(op.Operand(1).Reg));
            break;
        case Arm64Opcode::A64SysReg_TPIDR_EL0:
            Reg.Set64(Arm64Opcode::ARM64_REG_TPIDR_EL0, Reg.Get64(op.Operand(1).Reg));
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Msub(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 4 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && op.Operand(3).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg) && CRegisters::Is64bitReg(op.Operand(2).Reg) && CRegisters::Is64bitReg(op.Operand(3).Reg))
    {
        Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(3).Reg) - (Reg.Get64(op.Operand(1).Reg) * Reg.Get64(op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Mul(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg) && CRegisters::Is32bitReg(op.Operand(2).Reg))
    {
        Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) * Reg.Get32(op.Operand(2).Reg));
    }
    else if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg) && CRegisters::Is64bitReg(op.Operand(2).Reg))
    {
        Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) * Reg.Get64(op.Operand(2).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Neg(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg) && op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
    {
        Reg.Set32(op.Operand(0).Reg, -((int32_t)Reg.Get32(op.Operand(1).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Orr(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) | op.Operand(2).ImmVal);
        }
        else if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(2).Reg) && op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
        {
            Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) | Reg.Get64(op.Operand(2).Reg));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) | (uint32_t)op.Operand(2).ImmVal);
        }
        else if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(2).Reg) && op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
        {
            Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) | Reg.Get32(op.Operand(2).Reg));
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
}

void Arm64Op::Rbit(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        uint64_t x = Reg.Get64(op.Operand(1).Reg);

        //swap the byte order
        x = (((x & 0x00000000000000ffULL) << 56) |
            ((x & 0x000000000000ff00ULL) << 40) |
            ((x & 0x0000000000ff0000ULL) << 24) |
            ((x & 0x00000000ff000000ULL) << 8) |
            ((x & 0x000000ff00000000ULL) >> 8) |
            ((x & 0x0000ff0000000000ULL) >> 24) |
            ((x & 0x00ff000000000000ULL) >> 40) |
            ((x & 0xff00000000000000ULL) >> 56));

        // assign the correct nibble position
        x = ((x & 0xf0f0f0f0f0f0f0f0ULL) >> 4)
            | ((x & 0x0f0f0f0f0f0f0f0fULL) << 4);

        // assign the correct bit position
        x = ((x & 0x8888888888888888ULL) >> 3)
            | ((x & 0x4444444444444444ULL) >> 1)
            | ((x & 0x2222222222222222ULL) << 1)
            | ((x & 0x1111111111111111ULL) << 3);

        Reg.Set64(op.Operand(0).Reg, x);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ret(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 0)
    {
        CRegisters & Reg = core.Reg();
        Reg.Set64(Arm64Opcode::ARM64_REG_PC, Reg.Get64(Arm64Opcode::ARM64_REG_LR));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Stp(CPUExecutor & core, const Arm64Opcode &op)
{
    MemoryManagement & MMU = core.MMU();
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM)
    {
        if (op.Operand(0).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        uint64_t target_addr = Reg.Get64(op.Operand(2).mem.base) + op.Operand(2).mem.disp;
        if (op.Operand(2).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
        {
            MMU.Write64(target_addr, Reg.Get64(op.Operand(0).Reg));
            MMU.Write64(target_addr + 8, Reg.Get64(op.Operand(1).Reg));
        }
        else if (CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
        {
            MMU.Write32(target_addr, Reg.Get32(op.Operand(0).Reg));
            MMU.Write32(target_addr + 4, Reg.Get32(op.Operand(1).Reg));
        }
        else if (CRegisters::Is128bitReg(op.Operand(0).Reg) && CRegisters::Is128bitReg(op.Operand(1).Reg))
        {
            uint64_t data[4];

            Reg.Get128(op.Operand(0).Reg, data[0], data[1]);
            Reg.Get128(op.Operand(1).Reg, data[2], data[3]);

            if ((target_addr & 0x7) != 0)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            if (!MMU.WriteBytes(target_addr, (uint8_t*)&data[0], sizeof(data)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (op.WriteBack())
        {
            Reg.Set64(op.Operand(2).mem.base, op.Operands() == 4 ? target_addr + op.Operand(3).ImmVal : target_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sbfiz(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 4 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        int64_t lsb = op.Operand(2).ImmVal;
        int64_t width = op.Operand(3).ImmVal;

        if (lsb < 0 || lsb > 63)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if (width < 1 || width > 64)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        Reg.Set64(op.Operand(0).Reg, (int64_t)(Reg.Get64(op.Operand(1).Reg) << width) >> (width - lsb));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sbfx(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 4 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        int64_t lsb = op.Operand(2).ImmVal;
        int64_t width = op.Operand(3).ImmVal;
        if (lsb < 0 || lsb > 63)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if (width < 1 || width > 64)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }

        uint64_t signbit = ((uint64_t)1 << width) - 1;
        width = (64 - width);
        Reg.Set64(op.Operand(0).Reg, ((int64_t)(((Reg.Get64(op.Operand(1).Reg) >> lsb) & signbit) << width) >> width));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}
    
void Arm64Op::Str(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    MemoryManagement & MMU = core.MMU();

    if ((op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        uint64_t index = 0;
        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            if (CRegisters::Is64bitReg(op.Operand(1).mem.index))
            {
                index = Reg.Get64(op.Operand(1).mem.index);
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }

        if (op.Operand(1).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        uint64_t target_addr = 0;
        if (op.Operand(1).mem.base != Arm64Opcode::ARM64_REG_INVALID)
        {
            if (CRegisters::Is64bitReg(op.Operand(1).mem.base))
            {
                target_addr = Reg.Get64(op.Operand(1).mem.base) + op.Operand(1).mem.disp + index;
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
        }

        if (CRegisters::Is64bitReg(op.Operand(0).Reg))
        {
            MMU.Write64(target_addr, Reg.Get64(op.Operand(0).Reg));
        }
        else if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            MMU.Write32(target_addr, Reg.Get32(op.Operand(0).Reg));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (op.WriteBack() || (op.Operands() == 3 && op.Operand(2).ImmVal != 0))
        {
            if (CRegisters::Is64bitReg(op.Operand(1).mem.base))
            {
                Reg.Set64(op.Operand(1).mem.base, op.Operands() == 3 ? target_addr + op.Operand(2).ImmVal : target_addr);
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Strb(CPUExecutor & core, const Arm64Opcode &op)
{
    if ((op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM))
    {
        MemoryManagement & MMU = core.MMU();
        CRegisters & Reg = core.Reg();
        uint64_t target_addr = Reg.Get64(op.Operand(1).mem.base) + op.Operand(1).mem.disp;

        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            if (!MMU.Write16(target_addr, (uint16_t)Reg.Get32(op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (op.WriteBack())
        {
            Reg.Set64(op.Operand(1).mem.base, op.Operands() == 3 ? target_addr + op.Operand(2).ImmVal : target_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Strh(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        MemoryManagement & MMU = core.MMU();
        CRegisters & Reg = core.Reg();
        Arm64Opcode::arm64_reg reg_base = op.Operand(1).mem.base;
        uint64_t target_addr = Reg.Get64(reg_base) + op.Operand(1).mem.disp;
        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            if (!MMU.Write16(target_addr, (uint16_t)Reg.Get32(op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Stlxr(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM)
    {
        MemoryManagement & MMU = core.MMU();
        CRegisters & Reg = core.Reg();

        if (op.Operand(2).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (op.Operand(2).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        uint64_t store_addr = Reg.Get64(op.Operand(2).mem.base) + op.Operand(2).mem.disp;
        if (CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
        {
            if (!MMU.Write32(store_addr, Reg.Get32(op.Operand(1).Reg)))
            {
                Reg.Set32(op.Operand(0).Reg, 1);
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            Reg.Set32(op.Operand(0).Reg, 0);
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
}

void Arm64Op::Stur(CPUExecutor & core, const Arm64Opcode &op)
{
    MemoryManagement & MMU = core.MMU();
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t target_addr = Reg.Get64(op.Operand(1).mem.base) + op.Operand(1).mem.disp;

        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (CRegisters::Is64bitReg(op.Operand(0).Reg))
        {
            if (!MMU.Write64(target_addr, Reg.Get64(op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            if (!MMU.Write32(target_addr, Reg.Get32(op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sturb(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        MemoryManagement & MMU = core.MMU();
        CRegisters & Reg = core.Reg();
        Arm64Opcode::arm64_reg reg_base = op.Operand(1).mem.base;
        uint64_t target_addr = Reg.Get64(reg_base) + op.Operand(1).mem.disp;
        if (op.Operand(1).mem.index != Arm64Opcode::ARM64_REG_INVALID)
        {
            target_addr += Reg.Get64(op.Operand(1).mem.index);
        }

        if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            if (!MMU.Write8(target_addr, (uint8_t)Reg.Get32(op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
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
}

void Arm64Op::Svc(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 1 && op.Operand(0).type == Arm64Opcode::ARM64_OP_IMM)
    {
        core.ServiceCall((uint32_t)op.Operand(0).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sxtw(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
    {
        Reg.Set64(op.Operand(0).Reg, (int64_t)((int32_t)Reg.Get32(op.Operand(1).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Tbnz(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && CRegisters::Is32bitReg(op.Operand(0).Reg))
    {
        if (op.Operand(1).ImmVal > 0x1F)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        CRegisters & Reg = core.Reg();
        if ((Reg.Get32(op.Operand(0).Reg) & (1 << op.Operand(1).ImmVal)) != 0)
        {
            Reg.Set64(Arm64Opcode::ARM64_REG_PC, op.Operand(2).ImmVal);
            core.Jumped();
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Tbz(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && CRegisters::Is32bitReg(op.Operand(0).Reg))
    {
        if (op.Operand(1).ImmVal > 0x1F)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        CRegisters & Reg = core.Reg();
        if ((Reg.Get32(op.Operand(0).Reg) & (1 << op.Operand(1).ImmVal)) == 0)
        {
            Reg.Set64(Arm64Opcode::ARM64_REG_PC, op.Operand(2).ImmVal);
            core.Jumped();
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Tst(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    uint64_t result = 0;
    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        result = Reg.Get32(op.Operand(0).Reg) & op.Operand(1).ImmVal;
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    bool n = (result & 0x80000000) != 0;
    bool z = result == 0;

    Reg.SetConditionFlags(n, z, 0, 0);
}

void Arm64Op::Udiv(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg) && CRegisters::Is64bitReg(op.Operand(2).Reg))
    {
        uint64_t divisor = Reg.Get64(op.Operand(2).Reg);
        if (divisor == 0)
        {
            Reg.Set64(op.Operand(0).Reg, 0);
        }
        else
        {
            Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) / divisor);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Umaddl(CPUExecutor & core, const Arm64Opcode &op)
{
    if (op.Operands() == 4 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && op.Operand(3).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg) && CRegisters::Is32bitReg(op.Operand(2).Reg) && CRegisters::Is64bitReg(op.Operand(3).Reg))
    {
        CRegisters & Reg = core.Reg();
        Reg.Set64(op.Operand(0).Reg, (uint64_t)(Reg.Get32(op.Operand(1).Reg) * Reg.Get32(op.Operand(2).Reg))+ Reg.Get64(op.Operand(3).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Umulh(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG)
    {
        Reg.Set64(op.Operand(0).Reg, mullhi_u64(Reg.Get64(op.Operand(1).Reg), Reg.Get64(op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

