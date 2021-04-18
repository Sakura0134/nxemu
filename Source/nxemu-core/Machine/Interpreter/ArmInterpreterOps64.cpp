#include <nxemu-core\Machine\Interpreter\ArmInterpreterOps64.h>
#include <nxemu-core\hle\Memory\SystemThreadMemory.h>
#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\Maths.h>

void Arm64Op::Add(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        uint64_t a = Reg.Get64(Op.Operand(1).Reg), b = 0;
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            b = Op.Operand(2).ImmVal;
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
        {
            if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get64(Op.Operand(2).Reg);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_ASR && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = (int64_t)Reg.Get64(Op.Operand(2).Reg) >> Op.Operand(2).shift.value;
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get64(Op.Operand(2).Reg) << Op.Operand(2).shift.value;
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSR && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get64(Op.Operand(2).Reg) >> Op.Operand(2).shift.value;
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
        {
            if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_UXTW)
            {
                b = (uint32_t)(Reg.Get32(Op.Operand(2).Reg));
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_SXTW)
            {
                b = (int64_t)((int32_t)(Reg.Get32(Op.Operand(2).Reg)));
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_SXTW)
            {
               b = ((int64_t)(int32_t)Reg.Get32(Op.Operand(2).Reg)) << Op.Operand(2).shift.value;
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_UXTB)
            {
                b = (uint8_t)(Reg.Get32(Op.Operand(2).Reg) << Op.Operand(2).shift.value);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_UXTW)
            {
                b = (uint32_t)(Reg.Get32(Op.Operand(2).Reg) << Op.Operand(2).shift.value);
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
        uint64_t result = a + b;
        Reg.Set64(Op.Operand(0).Reg, result);
        if (Op.UpdateFlags())
        {
            bool n = (result & 0x8000000000000000) != 0;
            bool z = result == 0;
            bool c = result < a;
            bool v = (((result ^ a) & (a ^ b)) & 0x8000000000000000) != 0;

            Reg.SetConditionFlags(n, z, c, v);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        uint32_t a = Reg.Get32(Op.Operand(1).Reg), b = 0;
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
        {
            if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get32(Op.Operand(2).Reg);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_UXTB)
            {
                b = (uint8_t)(Reg.Get32(Op.Operand(2).Reg));
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = (Reg.Get32(Op.Operand(2).Reg) << Op.Operand(2).shift.value);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSR && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = (Reg.Get32(Op.Operand(2).Reg) >> Op.Operand(2).shift.value);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_ASR && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = ((int32_t)Reg.Get32(Op.Operand(2).Reg) >> Op.Operand(2).shift.value);
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            b = (uint32_t)Op.Operand(2).ImmVal;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        uint32_t result = a + b;
        Reg.Set32(Op.Operand(0).Reg, result);
        if (Op.UpdateFlags())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Adrp(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        IRegisters & Reg = Cpu.Reg();
        Reg.Set64(Op.Operand(0).Reg, Op.Operand(1).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::And(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) & Reg.Get64(Op.Operand(2).Reg));
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) & Op.Operand(2).ImmVal);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) & Reg.Get32(Op.Operand(2).Reg));
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) & Op.Operand(2).ImmVal);
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

void Arm64Op::Asr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, (int64_t)Reg.Get64(Op.Operand(1).Reg) >> Op.Operand(2).ImmVal);
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, (int32_t)Reg.Get32(Op.Operand(1).Reg) >> Op.Operand(2).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::B(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    if (Op.Operands() == 1 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_IMM)
    {
        Cpu.Jump(Op.Operand(0).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Bfi(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(3).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            int64_t lsb = Op.Operand(2).ImmVal;
            int64_t width = Op.Operand(3).ImmVal;

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
            uint64_t OrigialValue = Reg.Get64(Op.Operand(0).Reg) & (~(signbit << lsb));
            Reg.Set64(Op.Operand(0).Reg, OrigialValue | (Reg.Get64(Op.Operand(1).Reg) & signbit) << lsb);
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            int64_t lsb = Op.Operand(2).ImmVal;
            int64_t width = Op.Operand(3).ImmVal;

            if (lsb < 0 || lsb > 31)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            if (width < 1 || width > 32)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }

            uint32_t signbit = (1u << width) - 1;
            uint32_t OrigialValue = Reg.Get32(Op.Operand(0).Reg) & (~(signbit << lsb));
            Reg.Set32(Op.Operand(0).Reg, OrigialValue | (Reg.Get32(Op.Operand(1).Reg) & signbit) << lsb);
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

void Arm64Op::Bfxil(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(3).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        int64_t lsb = Op.Operand(2).ImmVal;
        int64_t width = Op.Operand(3).ImmVal;

        if (lsb < 0 || lsb > 31)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if (width < 1 || width > 32)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        uint32_t signbit = (1u << width) - 1;
        uint32_t OrigialValue = Reg.Get32(Op.Operand(0).Reg) & ~signbit;
        Reg.Set32(Op.Operand(0).Reg, OrigialValue | ((Reg.Get32(Op.Operand(1).Reg) >> lsb) & signbit));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Bic(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg) && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) & ~(Reg.Get64(Op.Operand(2).Reg)));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSR && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) & ~(Reg.Get32(Op.Operand(2).Reg) >> Op.Operand(2).shift.value));
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) & ~(Reg.Get32(Op.Operand(2).Reg)));
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

void Arm64Op::Bl(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 1 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_IMM)
    {
        Reg.Set64(Arm64Opcode::ARM64_REG_LR, Reg.Get64(Arm64Opcode::ARM64_REG_PC) + 4);
        Cpu.Jump(Op.Operand(0).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Blr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 1 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        Reg.Set64(Arm64Opcode::ARM64_REG_LR, Reg.Get64(Arm64Opcode::ARM64_REG_PC) + 4);
        Cpu.Jump(Reg.Get64(Op.Operand(0).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Br(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 1 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        Cpu.Jump(Reg.Get64(Op.Operand(0).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Cbnz(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
        {
            if (Reg.Get64(Op.Operand(0).Reg) != 0)
            {
                Cpu.Jump(Op.Operand(1).ImmVal);
            }
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            if (Reg.Get32(Op.Operand(0).Reg) != 0)
            {
                Cpu.Jump(Op.Operand(1).ImmVal);
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

void Arm64Op::Cbz(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (Reg.Get64(Op.Operand(0).Reg) == 0)
        {
            Cpu.Jump(Op.Operand(1).ImmVal);
        }
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (Reg.Get32(Op.Operand(0).Reg) == 0)
        {
            Cpu.Jump(Op.Operand(1).ImmVal);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ccmp(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
    {
        int64_t ImmVal;
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
        {
            ImmVal = Reg.Get32(Op.Operand(2).Reg);
        }
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            ImmVal = Op.Operand(2).ImmVal;
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
        if (Reg.ConditionSet(Op.cc()))
        {
            uint32_t a = Reg.Get32(Op.Operand(0).Reg);
            uint32_t b;
            if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
            {
               b = Reg.Get32(Op.Operand(1).Reg);
            }
            else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
            {
                b = (uint32_t)Op.Operand(1).ImmVal;
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
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
    {
        int64_t ImmVal;
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
        {
            ImmVal = Reg.Get64(Op.Operand(2).Reg);
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            ImmVal = Op.Operand(2).ImmVal;
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
        if (Reg.ConditionSet(Op.cc()))
        {
            uint64_t a = Reg.Get64(Op.Operand(0).Reg), b;
            if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
            {
                b = Reg.Get64(Op.Operand(1).Reg);
            }
            else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
            {
                b = Op.Operand(1).ImmVal;
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

void Arm64Op::Cinc(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get64(Op.Operand(1).Reg) + 1 : Reg.Get64(Op.Operand(1).Reg));
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get32(Op.Operand(1).Reg) + 1 : Reg.Get32(Op.Operand(1).Reg));
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

void Arm64Op::Clrex(CInterpreterCPU & /*core*/, const Arm64Opcode & /*Op*/)
{
}

void Arm64Op::Clz(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg,clz64(Reg.Get64(Op.Operand(1).Reg)));
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, clz32(Reg.Get32(Op.Operand(1).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Cmn(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
    {
        uint64_t a, b;
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            a = Reg.Get64(Op.Operand(0).Reg);
            b = Op.Operand(1).ImmVal;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }

        uint64_t result = a + b;
        bool n = (result & 0x8000000000000000) != 0;
        bool z = result == 0;
        bool c = result < a;
        bool v = (((result ^ a) & (~(a ^ b))) & 0x8000000000000000) != 0;
        Reg.SetConditionFlags(n, z, c, v);
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
    {
        uint32_t a, b;
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            a = Reg.Get32(Op.Operand(0).Reg);
            b = (uint32_t)Op.Operand(1).ImmVal;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }

        uint32_t result = a + b;
        bool n = (result & 0x80000000) != 0;
        bool z = result == 0;
        bool c = result < a;
        bool v = (((result ^ a) & (~(a ^ b))) & 0x80000000) != 0;
        Reg.SetConditionFlags(n, z, c, v);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
}

void Arm64Op::Cmp(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
    {
        uint64_t a = Reg.Get64(Op.Operand(0).Reg), b;
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            b = Reg.Get64(Op.Operand(1).Reg);
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_LSR && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            b = Reg.Get64(Op.Operand(1).Reg) >> Op.Operand(1).shift.value;
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_SXTW && Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
        {
            b = (int64_t)((int32_t)Reg.Get32(Op.Operand(1).Reg));
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            b = Op.Operand(1).ImmVal;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }

        uint64_t result = a - b;
        bool n = (result & 0x8000000000000000) != 0;
        bool z = result == 0;
        bool c = a >= b;
        bool v = (((result ^ a) & (a ^ b)) & 0x8000000000000000) != 0;
        Reg.SetConditionFlags(n, z, c, v);
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
    {
        uint32_t a = Reg.Get32(Op.Operand(0).Reg), b;
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            b = Reg.Get32(Op.Operand(1).Reg);
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_UXTB)
        {
            b = (uint8_t)Reg.Get32(Op.Operand(1).Reg);
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            b = (uint32_t)Op.Operand(1).ImmVal;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }

        uint32_t result = a - b;
        bool n = (result & 0x80000000) != 0;
        bool z = result == 0;
        bool c = a >= b;
        bool v = (((result ^ a) & (a ^ b)) & 0x80000000) != 0;
        Reg.SetConditionFlags(n, z, c, v);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
}

void Arm64Op::Cnt(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::IsVectorReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::IsVectorReg(Op.Operand(1).Reg))
    {
        uint64_t value = Reg.Get64Vector(Op.Operand(1).Reg, Op.Operand(1).VectorIndex, Op.Operand(1).Vess, Op.Operand(1).Vas);
        uint32_t x_lo = (uint32_t)(value & 0xFFFFFFFF), x_hi = (uint32_t)(value >> 32);
        x_lo = (x_lo & 0x55555555) + ((x_lo >> 1) & 0x55555555);
        x_lo = (x_lo & 0x33333333) + ((x_lo >> 2) & 0x33333333);
        x_lo = (x_lo & 0x0f0f0f0f) + ((x_lo >> 4) & 0x0f0f0f0f);
        x_hi = (x_hi & 0x55555555) + ((x_hi >> 1) & 0x55555555);
        x_hi = (x_hi & 0x33333333) + ((x_hi >> 2) & 0x33333333);
        x_hi = (x_hi & 0x0f0f0f0f) + ((x_hi >> 4) & 0x0f0f0f0f);
        Reg.Set64Vector(Op.Operand(0).Reg, Op.Operand(0).VectorIndex, Op.Operand(0).Vess, Op.Operand(0).Vas, (((uint64_t)x_hi << 32) | x_lo));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Csel(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) &&
        Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get64(Op.Operand(1).Reg) : Reg.Get64(Op.Operand(2).Reg));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) &&
        Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get32(Op.Operand(1).Reg) : Reg.Get32(Op.Operand(2).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Cset(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 1 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? 1 : 0);
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

void Arm64Op::Csetm(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 1 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? 0xFFFFFFFFFFFFFFFF : 0);
    }
    else if (Op.Operands() == 1 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? 0xFFFFFFFF : 0);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Csinc(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get64(Op.Operand(1).Reg) : (Reg.Get64(Op.Operand(2).Reg) + 1));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get32(Op.Operand(1).Reg) : (Reg.Get32(Op.Operand(2).Reg) + 1));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Csinv(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get64(Op.Operand(1).Reg) : ~Reg.Get64(Op.Operand(2).Reg));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get32(Op.Operand(1).Reg) : ~Reg.Get32(Op.Operand(2).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Dc(CInterpreterCPU & /*core*/, const Arm64Opcode& /*Op*/)
{
    //ignore cache ops
}

void Arm64Op::Dup(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::IsVectorReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        Reg.Set64Vector(Op.Operand(0).Reg, Op.Operand(0).VectorIndex, Op.Operand(0).Vess, Op.Operand(0).Vas, Reg.Get64(Op.Operand(1).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Dmb(CInterpreterCPU & /*Cpu*/, const Arm64Opcode & /*Op*/)
{
}

void Arm64Op::Dsb(CInterpreterCPU & /*core*/, const Arm64Opcode & /*Op*/)
{
}

void Arm64Op::Eor(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        uint64_t a = Reg.Get64(Op.Operand(1).Reg), b = 0;
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            b = Op.Operand(2).ImmVal;
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
        {
            if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get64(Op.Operand(2).Reg);
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
        Reg.Set64(Op.Operand(0).Reg, a ^ b);
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        uint32_t a = Reg.Get32(Op.Operand(1).Reg), b = 0;
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            b = (uint32_t)Op.Operand(2).ImmVal;
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
        {
            if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get32(Op.Operand(2).Reg);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = (Reg.Get32(Op.Operand(2).Reg) << Op.Operand(2).shift.value);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSR && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = (Reg.Get32(Op.Operand(2).Reg) >> Op.Operand(2).shift.value);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_ROR && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = (Reg.Get32(Op.Operand(2).Reg) >> Op.Operand(2).shift.value) | (Reg.Get32(Op.Operand(2).Reg) << (32 - Op.Operand(2).shift.value));
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
        Reg.Set32(Op.Operand(0).Reg, a ^ b);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Extr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(3).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
    {
        if (Op.Operand(3).ImmVal > 64)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        Reg.Set64(Op.Operand(0).Reg, (Reg.Get64(Op.Operand(2).Reg) >> Op.Operand(3).ImmVal) | (Reg.Get64(Op.Operand(1).Reg) << (64 - Op.Operand(3).ImmVal)));
    }
    else if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(3).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
    {
        if (Op.Operand(3).ImmVal > 32)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        Reg.Set32(Op.Operand(0).Reg, (Reg.Get32(Op.Operand(2).Reg) >> Op.Operand(3).ImmVal) | (Reg.Get32(Op.Operand(1).Reg) << (32 - Op.Operand(3).ImmVal)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fabs(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
    {
        float32_t val = Reg.Get32Float(Op.Operand(1).Reg);
        val.v &= 0x7fffffffLL;
        Reg.Set32Float(Op.Operand(0).Reg, val);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fadd(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set32Float(Op.Operand(0).Reg, f32_add(Reg.Get32Float(Op.Operand(1).Reg), Reg.Get32Float(Op.Operand(2).Reg)));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set64Float(Op.Operand(0).Reg, f64_add(Reg.Get64Float(Op.Operand(1).Reg), Reg.Get64Float(Op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fcmp(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg))
    {
        float64_t a = Reg.Get64Float(Op.Operand(0).Reg);
        float64_t b = Reg.Get64Float(Op.Operand(1).Reg);
        bool n = false, z = false, c = false, v = false;
        if (f64_eq(a, b))
        {
            z = true;
            c = true;
        }
        else if (f64_lt(a, b))
        {
            n = true;
        }
        else
        {
            c = true;
        }
        Reg.SetConditionFlags(n, z, c, v);
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg))
    {
        float32_t a = Reg.Get32Float(Op.Operand(0).Reg);
        float32_t b;
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
        {
            b = Reg.Get32Float(Op.Operand(1).Reg);
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_FP)
        {
            float f = (float)Op.Operand(1).fp;
            b.v = *((uint32_t*)&f);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        bool n = false, z = false, c = false, v = false;
        if (f32_eq(a, b))
        {
            z = true;
            c = true;
        }
        else if (f32_lt(a, b))
        {
            n = true;
        }
        else
        {
            c = true;
        }
        Reg.SetConditionFlags(n, z, c, v);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fcsel(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg) &&
        Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set32Float(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get32Float(Op.Operand(1).Reg) : Reg.Get32Float(Op.Operand(2).Reg));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg) &&
        Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set64Float(Op.Operand(0).Reg, Reg.ConditionSet(Op.cc()) ? Reg.Get64Float(Op.Operand(1).Reg) : Reg.Get64Float(Op.Operand(2).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fcvt(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
    {
        Reg.Set64Float(Op.Operand(0).Reg, f32_to_f64(Reg.Get32Float(Op.Operand(1).Reg)));
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg))
    {
        Reg.Set32Float(Op.Operand(0).Reg, f64_to_f32(Reg.Get64Float(Op.Operand(1).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fcvtzs(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
    {
        float32_t val = Reg.Get32Float(Op.Operand(1).Reg);
        bool sign = (val.v >> 31) != 0;
        Reg.Set32(Op.Operand(0).Reg, f32_to_i32(Reg.Get32Float(Op.Operand(1).Reg), (uint_fast8_t)(sign ? softfloat_round_max : softfloat_round_min), true));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg) &&
        Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
    {
        float32_t val = Reg.Get32Float(Op.Operand(1).Reg);
        bool sign = (val.v >> 31) != 0;
        Reg.Set32(Op.Operand(0).Reg, f32_to_i32(f32_mul(val, i32_to_f32((1 << Op.Operand(2).ImmVal))), (uint_fast8_t)(sign ? softfloat_round_max : softfloat_round_min), true));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg) &&
        Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
    {
        float64_t val = Reg.Get64Float(Op.Operand(1).Reg);
        bool sign = (val.v >> 63) != 0;
        Reg.Set32(Op.Operand(0).Reg, f64_to_i32(f64_mul(val, i64_to_f64(1ll << Op.Operand(2).ImmVal)), (uint_fast8_t)(sign ? softfloat_round_max : softfloat_round_min), true));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fcvtzu(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg) &&
        Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
    {
        float32_t val = Reg.Get32Float(Op.Operand(1).Reg);
        bool sign = (val.v >> 31) != 0;
        Reg.Set32(Op.Operand(0).Reg, f32_to_i32(f32_mul(val, i32_to_f32((1 << Op.Operand(2).ImmVal))), (uint_fast8_t)(sign ? softfloat_round_max : softfloat_round_min), true));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fmaxnm(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg) &&
        Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(2).Reg))
    {
        float32_t val1 = Reg.Get32Float(Op.Operand(1).Reg);
        float32_t val2 = Reg.Get32Float(Op.Operand(2).Reg);
        Reg.Set32Float(Op.Operand(0).Reg, f32_le(val1, val2) ? val2 : val1);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fminnm(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) &&
        Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg) &&
        Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(2).Reg))
    {
        float32_t val1 = Reg.Get32Float(Op.Operand(1).Reg);
        float32_t val2 = Reg.Get32Float(Op.Operand(2).Reg);
        Reg.Set32Float(Op.Operand(0).Reg, f32_le(val1, val2) ? val1 : val2);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fdiv(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set64Float(Op.Operand(0).Reg, f64_div(Reg.Get64Float(Op.Operand(1).Reg), Reg.Get64Float(Op.Operand(2).Reg)));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set32Float(Op.Operand(0).Reg, f32_div(Reg.Get32Float(Op.Operand(1).Reg), Reg.Get32Float(Op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fmov(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            float64_t reg;
            reg.v = Reg.Get64(Op.Operand(1).Reg);
            Reg.Set64Float(Op.Operand(0).Reg, reg);
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32Float(Op.Operand(1).Reg).v);
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            float32_t reg;
            reg.v = Reg.Get32(Op.Operand(1).Reg);
            Reg.Set32Float(Op.Operand(0).Reg, reg);
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_FP && Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg))
        {
            double f = (double)Op.Operand(1).fp;
            float64_t value;
            value.v = *((uint64_t *)&f);
            Reg.Set64Float(Op.Operand(0).Reg, value);
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_FP && Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg))
        {
            float f = (float)Op.Operand(1).fp;
            float32_t value;
            value.v = *((uint32_t *)&f);
            Reg.Set32Float(Op.Operand(0).Reg, value);
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

void Arm64Op::Fmul(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set64Float(Op.Operand(0).Reg, f64_mul(Reg.Get64Float(Op.Operand(1).Reg), Reg.Get64Float(Op.Operand(2).Reg)));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set32Float(Op.Operand(0).Reg, f32_mul(Reg.Get32Float(Op.Operand(1).Reg),Reg.Get32Float(Op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fneg(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg))
    {
        float64_t val = Reg.Get64Float(Op.Operand(1).Reg);
        val.v ^= 0x8000000000000000LL;
        Reg.Set64Float(Op.Operand(0).Reg, val);
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
    {
        float32_t val = Reg.Get32Float(Op.Operand(1).Reg);
        val.v ^= 0x80000000;
        Reg.Set32Float(Op.Operand(0).Reg, val);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fsqrt(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg))
    {
        float64_t val = Reg.Get64Float(Op.Operand(1).Reg);
        Reg.Set64Float(Op.Operand(0).Reg, f64_sqrt(val));
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
    {
        float32_t val = Reg.Get32Float(Op.Operand(1).Reg);
        Reg.Set32Float(Op.Operand(0).Reg, f32_sqrt(val));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Fsub(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set32Float(Op.Operand(0).Reg, f32_sub(Reg.Get32Float(Op.Operand(1).Reg), Reg.Get32Float(Op.Operand(2).Reg)));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(2).Reg))
    {
        Reg.Set64Float(Op.Operand(0).Reg, f64_sub(Reg.Get64Float(Op.Operand(1).Reg), Reg.Get64Float(Op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ins(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::IsVectorReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            Reg.Set64Vector(Op.Operand(0).Reg, Op.Operand(0).VectorIndex, Op.Operand(0).Vess, Op.Operand(0).Vas, Reg.Get64(Op.Operand(1).Reg));
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

void Arm64Op::Ldarb(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if ((Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM))
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint8_t value;
            if (!ThreadMemory.Read8(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldarh(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory& ThreadMemory = Cpu.ThreadMemory();

    if (Op.UpdateFlags() || Op.WriteBack()) { g_Notify->BreakPoint(__FILE__, __LINE__); }

    if ((Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        IRegisters & Reg = Cpu.Reg();
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint16_t value;
            if (!ThreadMemory.Read16(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack() || Op.Operands() == 3)
        {
            Reg.Set64(Op.Operand(1).mem.base, Op.Operands() == 3 ? target_addr + Op.Operand(2).ImmVal : target_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldaxr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
        {
            uint64_t value;
            if (!ThreadMemory.Read64(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            if (Op.Operand(0).Reg != Arm64Opcode::ARM64_REG_XZR)
            {
                Reg.Set64(Op.Operand(0).Reg, value);
            }
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint32_t value;
            if (!ThreadMemory.Read32(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldp(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM ||
        Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM && Op.Operand(3).type == Arm64Opcode::ARM64_OP_IMM)
    {
        uint64_t index = MemIndex(Op.Operand(2), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(2).mem.base) + Op.Operand(2).mem.disp + index;

        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            uint64_t value;
            if (!ThreadMemory.Read64(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(Op.Operand(0).Reg, value);

            if (!ThreadMemory.Read64(target_addr + 8, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(Op.Operand(1).Reg, value);
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            uint32_t value;
            if (!ThreadMemory.Read32(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);

            if (!ThreadMemory.Read32(target_addr + 4, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(1).Reg, value);
        }
        else if (Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg))
        {
            float64_t value;
            if (!ThreadMemory.Read64(target_addr, value.v)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64Float(Op.Operand(0).Reg, value);

            if (!ThreadMemory.Read64(target_addr + 8, value.v)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64Float(Op.Operand(1).Reg, value);
        }
        else if (Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
        {
            float32_t value;
            if (!ThreadMemory.Read32(target_addr, value.v)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32Float(Op.Operand(0).Reg, value);

            if (!ThreadMemory.Read32(target_addr + 4, value.v)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32Float(Op.Operand(1).Reg, value);
        }
        else if (Arm64Opcode::Is128bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is128bitReg(Op.Operand(1).Reg))
        {
            uint64_t valueHi, valueLo;
            if (!ThreadMemory.Read64(target_addr + 0x00, valueHi)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            if (!ThreadMemory.Read64(target_addr + 0x08, valueLo)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set128(Op.Operand(0).Reg, valueHi, valueLo);

            if (!ThreadMemory.Read64(target_addr + 0x10, valueHi)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            if (!ThreadMemory.Read64(target_addr + 0x18, valueLo)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set128(Op.Operand(1).Reg, valueHi, valueLo);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (Op.WriteBack() || Op.Operands() == 4)
        {
            Reg.Set64(Op.Operand(2).mem.base, Op.Operands() == 4 ? target_addr + Op.Operand(3).ImmVal : target_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if ((Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr;
        if (Arm64Opcode::Is64bitReg(Op.Operand(1).mem.base))
        {
            target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
        {
            uint64_t value;
            if (!ThreadMemory.Read64(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(Op.Operand(0).Reg, value);
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint32_t value;
            if (!ThreadMemory.Read32(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);
        }
        else if (Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg))
        {
            float64_t value;
            if (!ThreadMemory.Read64(target_addr, value.v)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64Float(Op.Operand(0).Reg, value);
        }
        else if (Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg))
        {
            float32_t value;
            if (!ThreadMemory.Read32(target_addr, value.v)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32Float(Op.Operand(0).Reg, value);
        }
        else if (Arm64Opcode::Is128bitReg(Op.Operand(0).Reg))
        {
            uint64_t value[2];
            if (!ThreadMemory.ReadBytes(target_addr, (uint8_t *)&value[0], sizeof(value))) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set128(Op.Operand(0).Reg, value[0], value[1]);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack() || Op.Operands() == 3)
        {
            if (Arm64Opcode::Is64bitReg(Op.Operand(1).mem.base))
            {
                Reg.Set64(Op.Operand(1).mem.base, Op.Operands() == 3 ? target_addr + Op.Operand(2).ImmVal : target_addr);
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

void Arm64Op::Ldrb(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if ((Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint8_t value;
            if (!ThreadMemory.Read8(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack() || Op.Operands() == 3)
        {
            Reg.Set64(Op.Operand(1).mem.base, Op.Operands() == 3 ? target_addr + Op.Operand(2).ImmVal : target_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldrh(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if ((Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint16_t value;
            if (!ThreadMemory.Read16(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack() || Op.Operands() == 3)
        {
            Reg.Set64(Op.Operand(1).mem.base, Op.Operands() == 3 ? target_addr + Op.Operand(2).ImmVal : target_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldrsb(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;

        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint8_t value;
            if (!ThreadMemory.Read8(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, (uint32_t)((int32_t)((int8_t)value)));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldrsw(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;

        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
        {
            uint32_t value;
            if (!ThreadMemory.Read32(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(Op.Operand(0).Reg, (uint64_t)((int64_t)((int32_t)value)));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldur(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
        {
            uint64_t value;
            if (!ThreadMemory.Read64(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(Op.Operand(0).Reg, value);
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint32_t value;
            if (!ThreadMemory.Read32(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldurb(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint8_t value;
            if (!ThreadMemory.Read8(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldurh(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            uint16_t value;
            if (!ThreadMemory.Read16(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set32(Op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldursw(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
        {
            uint32_t value;
            if (!ThreadMemory.Read32(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(Op.Operand(0).Reg, (int64_t)((int32_t)value));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ldxr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr;
        if (Arm64Opcode::Is64bitReg(Op.Operand(1).mem.base))
        {
            target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
        {
            uint64_t value;
            if (!ThreadMemory.Read64(target_addr, value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
            Reg.Set64(Op.Operand(0).Reg, value);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Lsl(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) << Reg.Get64(Op.Operand(2).Reg));
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) << Op.Operand(2).ImmVal);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) << Reg.Get32(Op.Operand(2).Reg));
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) << Op.Operand(2).ImmVal);
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

void Arm64Op::Lsr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) >> Reg.Get64(Op.Operand(2).Reg));
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) >> Reg.Get32(Op.Operand(2).Reg));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) >> Op.Operand(2).ImmVal);
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) >> Op.Operand(2).ImmVal);
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

void Arm64Op::Madd(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(3).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(3).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, (Reg.Get64(Op.Operand(1).Reg) * Reg.Get64(Op.Operand(2).Reg) + Reg.Get64(Op.Operand(3).Reg)));
    }
    else if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(3).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(3).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, (Reg.Get32(Op.Operand(1).Reg) * Reg.Get32(Op.Operand(2).Reg) + Reg.Get32(Op.Operand(3).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Mov(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            if (Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg));
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            if (Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg));
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::IsVectorReg(Op.Operand(0).Reg) && Arm64Opcode::IsVectorReg(Op.Operand(1).Reg))
        {
            if (Op.Operand(0).Vas == Arm64Opcode::ARM64_VAS_INVALID && Op.Operand(0).Vess == Arm64Opcode::ARM64_VESS_INVALID &&
                Op.Operand(1).Vas == Arm64Opcode::ARM64_VAS_INVALID && Op.Operand(1).Vess == Arm64Opcode::ARM64_VESS_INVALID)
            {
                uint64_t data[2];

                Reg.Get128(Op.Operand(1).Reg, data[0], data[1]);
                Reg.Set128(Op.Operand(0).Reg, data[0], data[1]);
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

void Arm64Op::Movi(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && Arm64Opcode::IsVectorReg(Op.Operand(0).Reg))
    {
        Reg.Set64Vector(Op.Operand(0).Reg, Op.Operand(0).VectorIndex, Op.Operand(0).Vess, Op.Operand(0).Vas, Op.Operand(1).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Movk(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
    {
        if (Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set64(Op.Operand(0).Reg, (Reg.Get64(Op.Operand(0).Reg) & ~((uint64_t)0xFFFF << Op.Operand(1).shift.value)) | ((uint64_t)(Op.Operand(1).ImmVal & 0xFFFF) << Op.Operand(1).shift.value));
        }
        else if (Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set64(Op.Operand(0).Reg, (Reg.Get64(Op.Operand(0).Reg) & ~0xFFFF) | (uint16_t)(Op.Operand(1).ImmVal & 0xFFFF));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
    {
        if (Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set32(Op.Operand(0).Reg, (Reg.Get32(Op.Operand(0).Reg) & ~0xFFFF) | (uint16_t)(Op.Operand(1).ImmVal & 0xFFFF));
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

void Arm64Op::Movn(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
    {
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(Op.Operand(0).Reg, ~Op.Operand(1).ImmVal);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
    {
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set32(Op.Operand(0).Reg, (uint32_t)(~Op.Operand(1).ImmVal));
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

void Arm64Op::Movz(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(Op.Operand(0).Reg, Op.Operand(1).ImmVal);
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set32(Op.Operand(0).Reg, (uint32_t)Op.Operand(1).ImmVal);
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

void Arm64Op::Mrs(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG_MRS)
    {
        switch (Op.Operand(1).SysReg)
        {
        case Arm64Opcode::A64SysReg_TPIDRRO_EL0:
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Arm64Opcode::ARM64_REG_TPIDRRO_EL0));
            break;
        case Arm64Opcode::A64SysReg_CNTPCT_EL0:
            Reg.Set64(Op.Operand(0).Reg, Cpu.GetCycleCount());
            break;
        case Arm64Opcode::A64SysReg_FPCR:
            Reg.Set64(Op.Operand(0).Reg, Reg.GetFPCR());
            break;
        case Arm64Opcode::A64SysReg_FPSR:
            Reg.Set64(Op.Operand(0).Reg, Reg.GetFPSR());
            break;
        case Arm64Opcode::A64SysReg_CTR_EL0:
            Reg.Set64(Op.Operand(0).Reg, 0x8444C004);
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

void Arm64Op::Msr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG_MSR && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        switch (Op.Operand(0).SysReg)
        {
        case Arm64Opcode::A64SysReg_FPCR:
            Reg.SetFPCR((uint32_t)Reg.Get64(Op.Operand(1).Reg));
            break;
        case Arm64Opcode::A64SysReg_FPSR:
            Reg.SetFPSR((uint32_t)Reg.Get64(Op.Operand(1).Reg));
            break;
        case Arm64Opcode::A64SysReg_TPIDR_EL0:
            Reg.Set64(Arm64Opcode::ARM64_REG_TPIDR_EL0, Reg.Get64(Op.Operand(1).Reg));
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

void Arm64Op::Msub(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(3).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(3).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(3).Reg) - (Reg.Get64(Op.Operand(1).Reg) * Reg.Get64(Op.Operand(2).Reg)));
    }
    else if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(3).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(3).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(3).Reg) - (Reg.Get32(Op.Operand(1).Reg) * Reg.Get32(Op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Mul(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) * Reg.Get64(Op.Operand(2).Reg));
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) * Reg.Get32(Op.Operand(2).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Mvn(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, ~(Reg.Get64(Op.Operand(1).Reg)));
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        Reg.Set32(Op.Operand(0).Reg, ~(Reg.Get32(Op.Operand(1).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Neg(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
    {
        Reg.Set64(Op.Operand(0).Reg, -((int64_t)Reg.Get64(Op.Operand(1).Reg)));
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
    {
        Reg.Set64(Op.Operand(0).Reg, -((int64_t)(Reg.Get64(Op.Operand(1).Reg) << Op.Operand(1).shift.value)));
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_LSR && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
    {
        Reg.Set64(Op.Operand(0).Reg, -((int64_t)(Reg.Get64(Op.Operand(1).Reg) >> Op.Operand(1).shift.value)));
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(1).Extend == Arm64Opcode::ARM64_EXT_INVALID)
    {
        Reg.Set32(Op.Operand(0).Reg, -((int32_t)Reg.Get32(Op.Operand(1).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Orn(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) | ~Reg.Get32(Op.Operand(2).Reg));
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

void Arm64Op::Orr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) | Op.Operand(2).ImmVal);
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg) && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) | (Reg.Get64(Op.Operand(2).Reg) << Op.Operand(2).shift.value));
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg) && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) | Reg.Get64(Op.Operand(2).Reg));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        uint32_t a = Reg.Get32(Op.Operand(1).Reg), b = 0;
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            b = (uint32_t)Op.Operand(2).ImmVal;
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
        {
            if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get32(Op.Operand(2).Reg);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get32(Op.Operand(2).Reg) << Op.Operand(2).shift.value;
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSR && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get32(Op.Operand(2).Reg) >> Op.Operand(2).shift.value;
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
        Reg.Set32(Op.Operand(0).Reg, a | b);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Rbit(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        uint64_t x = Reg.Get64(Op.Operand(1).Reg);

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

        Reg.Set64(Op.Operand(0).Reg, x);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ret(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 0)
    {
        Cpu.Jump(Reg.Get64(Arm64Opcode::ARM64_REG_LR));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ror(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) && Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            uint32_t value = Reg.Get32(Op.Operand(1).Reg);
            uint32_t size = Reg.Get32(Op.Operand(2).Reg);
            if (size > 32)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            Reg.Set32(Op.Operand(0).Reg, (value >> size) | (value << (32 - size)));
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

void Arm64Op::Stp(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if ((Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM) ||
        (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM && Op.Operand(3).type == Arm64Opcode::ARM64_OP_IMM))
    {
        uint64_t index = MemIndex(Op.Operand(2), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(2).mem.base) + Op.Operand(2).mem.disp + index;
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            ThreadMemory.Write64(target_addr, Reg.Get64(Op.Operand(0).Reg));
            ThreadMemory.Write64(target_addr + 8, Reg.Get64(Op.Operand(1).Reg));
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            ThreadMemory.Write32(target_addr, Reg.Get32(Op.Operand(0).Reg));
            ThreadMemory.Write32(target_addr + 4, Reg.Get32(Op.Operand(1).Reg));
        }
        else if (Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitFloatReg(Op.Operand(1).Reg))
        {
            ThreadMemory.Write64(target_addr, Reg.Get64Float(Op.Operand(0).Reg).v);
            ThreadMemory.Write64(target_addr + 8, Reg.Get64Float(Op.Operand(1).Reg).v);
        }
        else if (Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
        {
            ThreadMemory.Write32(target_addr, Reg.Get32Float(Op.Operand(0).Reg).v);
            ThreadMemory.Write32(target_addr + 4, Reg.Get32Float(Op.Operand(1).Reg).v);
        }
        else if (Arm64Opcode::Is128bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is128bitReg(Op.Operand(1).Reg))
        {
            uint64_t data[4];

            Reg.Get128(Op.Operand(0).Reg, data[0], data[1]);
            Reg.Get128(Op.Operand(1).Reg, data[2], data[3]);

            if (!ThreadMemory.WriteBytes(target_addr, (uint8_t*)&data[0], sizeof(data)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack() || (Op.Operands() == 4 && Op.Operand(3).ImmVal != 0))
        {
            Reg.Set64(Op.Operand(2).mem.base, Op.Operands() == 4 ? target_addr + Op.Operand(3).ImmVal : target_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sbfiz(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        int64_t lsb = Op.Operand(2).ImmVal;
        int64_t width = Op.Operand(3).ImmVal;

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
        Reg.Set64(Op.Operand(0).Reg, (int64_t)(Reg.Get64(Op.Operand(1).Reg) << width) >> (width - lsb));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sbfx(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        int64_t lsb = Op.Operand(2).ImmVal;
        int64_t width = Op.Operand(3).ImmVal;
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
        Reg.Set64(Op.Operand(0).Reg, ((int64_t)(((Reg.Get64(Op.Operand(1).Reg) >> lsb) & signbit) << width) >> width));
    }
    else if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        int64_t lsb = Op.Operand(2).ImmVal;
        int64_t width = Op.Operand(3).ImmVal;
        if (lsb < 0 || lsb > 31)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if (width < 1 || width > 32)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }

        uint32_t signbit = (1u << width) - 1;
        width = (32 - width);
        Reg.Set32(Op.Operand(0).Reg, ((int32_t)(((Reg.Get32(Op.Operand(1).Reg) >> lsb) & signbit) << width) >> width));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Scvtf(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        Reg.Set32Float(Op.Operand(0).Reg, i32_to_f32(Reg.Get32(Op.Operand(1).Reg)));
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
    {
        Reg.Set32Float(Op.Operand(0).Reg, i32_to_f32(Reg.Get32Float(Op.Operand(1).Reg).v));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sdiv(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, (int64_t)Reg.Get64(Op.Operand(1).Reg) / (int64_t)Reg.Get64(Op.Operand(2).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Smaddl(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(3).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(3).Reg))
    {
        IRegisters & Reg = Cpu.Reg();
        Reg.Set64(Op.Operand(0).Reg, (uint64_t)((int32_t)Reg.Get32(Op.Operand(1).Reg) * (int32_t)Reg.Get32(Op.Operand(2).Reg)) + Reg.Get64(Op.Operand(3).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Smulh(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, mullhi_s64(Reg.Get64(Op.Operand(1).Reg), Reg.Get64(Op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Smull(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, (int64_t)((int32_t)Reg.Get32(Op.Operand(1).Reg)) * ((int64_t)(int32_t)Reg.Get32(Op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Stlr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            if (!ThreadMemory.Write32(target_addr, Reg.Get32(Op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Stlrb(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;

        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            if (!ThreadMemory.Write8(target_addr, (uint8_t)Reg.Get32(Op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Str(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if ((Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = 0;
        if (Op.Operand(1).mem.base != Arm64Opcode::ARM64_REG_INVALID)
        {
            if (Arm64Opcode::Is64bitReg(Op.Operand(1).mem.base))
            {
                target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
        }

        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
        {
            ThreadMemory.Write64(target_addr, Reg.Get64(Op.Operand(0).Reg));
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            ThreadMemory.Write32(target_addr, Reg.Get32(Op.Operand(0).Reg));
        }
        else if (Arm64Opcode::Is64bitFloatReg(Op.Operand(0).Reg))
        {
            ThreadMemory.Write64(target_addr, Reg.Get64Float(Op.Operand(0).Reg).v);
        }
        else if (Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg))
        {
            ThreadMemory.Write32(target_addr, Reg.Get32Float(Op.Operand(0).Reg).v);
        }
        else if (Arm64Opcode::Is128bitReg(Op.Operand(0).Reg))
        {
            uint64_t valueHi, valueLo;
            Reg.Get128(Op.Operand(0).Reg, valueHi, valueLo);
            if (!ThreadMemory.Write64(target_addr + 0x00, valueHi))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            ThreadMemory.Write64(target_addr + 0x08, valueLo);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack() || (Op.Operands() == 3 && Op.Operand(2).ImmVal != 0))
        {
            if (Arm64Opcode::Is64bitReg(Op.Operand(1).mem.base))
            {
                Reg.Set64(Op.Operand(1).mem.base, Op.Operands() == 3 ? target_addr + Op.Operand(2).ImmVal : target_addr);
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

void Arm64Op::Strb(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if ((Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;

        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            if (!ThreadMemory.Write8(target_addr, (uint8_t)Reg.Get32(Op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack() || Op.Operands() == 3)
        {
            Reg.Set64(Op.Operand(1).mem.base, Op.Operands() == 3 ? target_addr + Op.Operand(2).ImmVal : target_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Strh(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if ((Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM) ||
        (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM))
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;

        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            if (!ThreadMemory.Write16(target_addr, (uint16_t)Reg.Get32(Op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack() || Op.Operands() == 3)
        {
            Reg.Set64(Op.Operand(1).mem.base, Op.Operands() == 3 ? target_addr + Op.Operand(2).ImmVal : target_addr);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Stlrh(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;

        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            if (!ThreadMemory.Write16(target_addr, (uint16_t)Reg.Get32(Op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sub(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        uint64_t a = Reg.Get64(Op.Operand(1).Reg), b = 0;
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            b = Op.Operand(2).ImmVal;
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
        {
            if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get64(Op.Operand(2).Reg);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_LSL && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get64(Op.Operand(2).Reg) << Op.Operand(2).shift.value;
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) &&
            Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_UXTW)
        {
            b = Reg.Get32(Op.Operand(2).Reg);
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) &&
            Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_SXTW)
        {
            b = (int64_t)((int32_t)Reg.Get32(Op.Operand(2).Reg));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        uint64_t result = a - b;
        Reg.Set64(Op.Operand(0).Reg, result);
        if (Op.UpdateFlags())
        {
            bool n = (result & 0x8000000000000000) != 0;
            bool z = result == 0;
            bool c = a >= b; //if the result of a subtraction is positive or zero
            bool v = ((((a ^ b) & (a ^ result)) >> 20) & 0x80000000) != 0;

            Reg.SetConditionFlags(n, z, c, v);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        uint32_t a = Reg.Get32(Op.Operand(1).Reg), b = 0;
        if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            b = (uint32_t)(Op.Operand(2).ImmVal);
        }
        else if (Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
        {
            if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                b = Reg.Get32(Op.Operand(2).Reg);
            }
            else if (Op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && Op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_UXTB)
            {
                b = (uint8_t)Reg.Get32(Op.Operand(2).Reg);
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
        Reg.Set32(Op.Operand(0).Reg, result);
        if (Op.UpdateFlags())
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

void Arm64Op::Stlxr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(2), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(2).mem.base) + Op.Operand(2).mem.disp + index;
        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            if (!ThreadMemory.Write64(target_addr, Reg.Get64(Op.Operand(1).Reg)))
            {
                Reg.Set32(Op.Operand(0).Reg, 1);
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            Reg.Set32(Op.Operand(0).Reg, 0);
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            if (!ThreadMemory.Write32(target_addr, Reg.Get32(Op.Operand(1).Reg)))
            {
                Reg.Set32(Op.Operand(0).Reg, 1);
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            Reg.Set32(Op.Operand(0).Reg, 0);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Stur(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;

        if (Arm64Opcode::Is128bitReg(Op.Operand(0).Reg))
        {
            uint64_t valueHi, valueLo;
            Reg.Get128(Op.Operand(0).Reg, valueHi, valueLo);
            if (!ThreadMemory.Write64(target_addr + 0x00, valueHi))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            if (!ThreadMemory.Write64(target_addr + 0x08, valueLo))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
        {
            if (!ThreadMemory.Write64(target_addr, Reg.Get64(Op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            if (!ThreadMemory.Write32(target_addr, Reg.Get32(Op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sturb(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;

        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            if (!ThreadMemory.Write8(target_addr, (uint8_t)Reg.Get32(Op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sturh(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(1), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(1).mem.base) + Op.Operand(1).mem.disp + index;

        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            if (!ThreadMemory.Write16(target_addr, (uint16_t)Reg.Get32(Op.Operand(0).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Stxr(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    CSystemThreadMemory & ThreadMemory = Cpu.ThreadMemory();
    IRegisters & Reg = Cpu.Reg();

    if (Op.UpdateFlags()) { g_Notify->BreakPoint(__FILE__, __LINE__); }

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_MEM)
    {
        uint64_t index = MemIndex(Op.Operand(2), Reg);
        uint64_t target_addr = Reg.Get64(Op.Operand(2).mem.base) + Op.Operand(2).mem.disp + index;

        if (Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            if (!ThreadMemory.Write64(target_addr, Reg.Get64(Op.Operand(1).Reg)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
        {
            Reg.Set32(Op.Operand(0).Reg, 0);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (Op.WriteBack())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Svc(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    if (Op.Operands() == 1 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_IMM)
    {
        Cpu.Thread().ServiceCall((uint32_t)Op.Operand(0).ImmVal);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Sxtw(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, (int64_t)((int32_t)Reg.Get32(Op.Operand(1).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Tbnz(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
    {
        if (Op.Operand(1).ImmVal > 0x3F)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if ((Reg.Get64(Op.Operand(0).Reg) & ((uint64_t)1 << Op.Operand(1).ImmVal)) != 0)
        {
            Cpu.Jump(Op.Operand(2).ImmVal);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
    {
        if (Op.Operand(1).ImmVal > 0x1F)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if ((Reg.Get32(Op.Operand(0).Reg) & (1 << Op.Operand(1).ImmVal)) != 0)
        {
            Cpu.Jump(Op.Operand(2).ImmVal);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Tbz(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
    {
        if (Op.Operand(1).ImmVal > 0x3F)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if ((Reg.Get64(Op.Operand(0).Reg) & ((uint64_t)1 << Op.Operand(1).ImmVal)) == 0)
        {
            Cpu.Jump(Op.Operand(2).ImmVal);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
    {
        if (Op.Operand(1).ImmVal > 0x1F)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if ((Reg.Get32(Op.Operand(0).Reg) & (1 << Op.Operand(1).ImmVal)) == 0)
        {
            Cpu.Jump(Op.Operand(2).ImmVal);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Tst(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is64bitReg(Op.Operand(0).Reg))
    {
        uint64_t result = 0;
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            result = Reg.Get64(Op.Operand(0).Reg) & Op.Operand(1).ImmVal;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        bool n = (result & 0x8000000000000000) != 0;
        bool z = result == 0;

        Reg.SetConditionFlags(n, z, 0, 0);
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(0).Reg))
    {
        uint32_t result = 0;
        if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            result = Reg.Get32(Op.Operand(0).Reg) & Reg.Get32(Op.Operand(1).Reg);
        }
        else if (Op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
        {
            result = Reg.Get32(Op.Operand(0).Reg) & Op.Operand(1).ImmVal;
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
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Uaddlv(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is16bitSimdReg(Op.Operand(0).Reg) && Arm64Opcode::IsVectorReg(Op.Operand(1).Reg) && Op.Operand(1).Vas == Arm64Opcode::ARM64_VAS_8B)
    {
        uint64_t value = Reg.Get64Vector(Op.Operand(1).Reg, Op.Operand(1).VectorIndex, Op.Operand(1).Vess, Op.Operand(1).Vas);
        uint8_t * src = (uint8_t *)&value;
        uint16_t add_value = src[0];
        add_value += src[1];
        add_value += src[2];
        add_value += src[3];
        add_value += src[4];
        add_value += src[5];
        add_value += src[6];
        add_value += src[7];
        Reg.Set16Simd(Op.Operand(0).Reg, add_value);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return;
}

void Arm64Op::Ubfiz(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
    {
        int64_t lsb = Op.Operand(2).ImmVal;
        int64_t width = Op.Operand(3).ImmVal;

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
        Reg.Set64(Op.Operand(0).Reg, (Reg.Get64(Op.Operand(1).Reg) & signbit) << lsb);
    }
    else if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        int64_t lsb = Op.Operand(2).ImmVal;
        int64_t width = Op.Operand(3).ImmVal;

        if (lsb < 0 || lsb > 31)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if (width < 1 || width > 32)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        uint32_t signbit = (1u << width) - 1;
        Reg.Set32(Op.Operand(0).Reg, (Reg.Get32(Op.Operand(1).Reg) & signbit) << lsb);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Ubfx(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM && Op.Operand(3).type == Arm64Opcode::ARM64_OP_IMM)
    {
        if (Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg))
        {
            int64_t lsb = Op.Operand(2).ImmVal;
            int64_t width = Op.Operand(3).ImmVal;
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
            Reg.Set64(Op.Operand(0).Reg, (Reg.Get64(Op.Operand(1).Reg) >> lsb) & signbit);
        }
        else if (Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
        {
            int64_t lsb = Op.Operand(2).ImmVal;
            int64_t width = Op.Operand(3).ImmVal;
            if (lsb < 0 || lsb > 31)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            if (width < 1 || width > 32)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }

            uint32_t signbit = (1u << width) - 1;
            Reg.Set32(Op.Operand(0).Reg, (Reg.Get32(Op.Operand(1).Reg) >> lsb) & signbit);
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

void Arm64Op::Ucvtf(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg))
    {
        float32_t val;
        val.v = Reg.Get32(Op.Operand(1).Reg);
        Reg.Set32Float(Op.Operand(0).Reg, val);
    }
    else if (Op.Operands() == 2 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitFloatReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitFloatReg(Op.Operand(1).Reg))
    {
        float32_t val = Reg.Get32Float(Op.Operand(1).Reg);
        Reg.Set32Float(Op.Operand(0).Reg, i32_to_f32(val.v));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Udiv(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();
    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(2).Reg))
    {
        uint64_t divisor = Reg.Get64(Op.Operand(2).Reg);
        if (divisor == 0)
        {
            Reg.Set64(Op.Operand(0).Reg, 0);
        }
        else
        {
            Reg.Set64(Op.Operand(0).Reg, Reg.Get64(Op.Operand(1).Reg) / divisor);
        }
    }
    else if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is32bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
    {
        uint32_t divisor = Reg.Get32(Op.Operand(2).Reg);
        if (divisor == 0)
        {
            Reg.Set32(Op.Operand(0).Reg, 0);
        }
        else
        {
            Reg.Set32(Op.Operand(0).Reg, Reg.Get32(Op.Operand(1).Reg) / divisor);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Umaddl(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters& Reg = Cpu.Reg();

    if (Op.Operands() == 4 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(3).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg) && Arm64Opcode::Is64bitReg(Op.Operand(3).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, (uint64_t)(Reg.Get32(Op.Operand(1).Reg) * Reg.Get32(Op.Operand(2).Reg))+ Reg.Get64(Op.Operand(3).Reg));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Umulh(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG)
    {
        Reg.Set64(Op.Operand(0).Reg, mullhi_u64(Reg.Get64(Op.Operand(1).Reg), Reg.Get64(Op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void Arm64Op::Umull(CInterpreterCPU & Cpu, const Arm64Opcode & Op)
{
    IRegisters & Reg = Cpu.Reg();

    if (Op.Operands() == 3 && Op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && Op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        Arm64Opcode::Is64bitReg(Op.Operand(0).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(1).Reg) && Arm64Opcode::Is32bitReg(Op.Operand(2).Reg))
    {
        Reg.Set64(Op.Operand(0).Reg, (uint64_t)(Reg.Get32(Op.Operand(1).Reg)) * (uint64_t)(Reg.Get32(Op.Operand(2).Reg)));
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

uint64_t Arm64Op::MemIndex(const Arm64Opcode::MCOperand & Operand, IRegisters & Reg)
{
    uint64_t index = 0;
    if (Operand.mem.index != Arm64Opcode::ARM64_REG_INVALID)
    {
        if (Arm64Opcode::Is64bitReg(Operand.mem.index) && Operand.shift.type == Arm64Opcode::ARM64_SFT_INVALID && Operand.Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            index = Reg.Get64(Operand.mem.index);
        }
        else if (Arm64Opcode::Is32bitReg(Operand.mem.index) && Operand.shift.type == Arm64Opcode::ARM64_SFT_INVALID && Operand.Extend == Arm64Opcode::ARM64_EXT_UXTW)
        {
            index = Reg.Get32(Operand.mem.index);
        }
        else if (Arm64Opcode::Is32bitReg(Operand.mem.index) && Operand.shift.type == Arm64Opcode::ARM64_SFT_INVALID && Operand.Extend == Arm64Opcode::ARM64_EXT_SXTW)
        {
            index = ((int64_t)(int32_t)Reg.Get32(Operand.mem.index));
        }
        else if (Arm64Opcode::Is64bitReg(Operand.mem.index) && Operand.shift.type == Arm64Opcode::ARM64_SFT_LSL && Operand.Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            index = Reg.Get64(Operand.mem.index) << Operand.shift.value;
        }
        else if (Arm64Opcode::Is32bitReg(Operand.mem.index) && Operand.shift.type == Arm64Opcode::ARM64_SFT_LSL && Operand.Extend == Arm64Opcode::ARM64_EXT_UXTW)
        {
            index = ((uint64_t)Reg.Get32(Operand.mem.index)) << Operand.shift.value;
        }
        else if (Arm64Opcode::Is32bitReg(Operand.mem.index) && Operand.shift.type == Arm64Opcode::ARM64_SFT_LSL && Operand.Extend == Arm64Opcode::ARM64_EXT_SXTW)
        {
            index = ((int64_t)(int32_t)Reg.Get32(Operand.mem.index)) << Operand.shift.value;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (Operand.shift.type != Arm64Opcode::ARM64_SFT_INVALID || Operand.Extend != Arm64Opcode::ARM64_EXT_INVALID)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return index;
}
