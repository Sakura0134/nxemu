#include <nxemu-core\Machine\Interpreter\ArmInterpreterOps64.h>
#include <nxemu-core\SystemGlobals.h>

void Arm64Op::Add(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(2).Reg))
        {
            if (op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
            {
                Reg.Set64(op.Operand(0).Reg, Reg.Get64(op.Operand(1).Reg) + Reg.Get64(op.Operand(2).Reg));
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
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
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
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    if (op.Operand(1).shift.type != Arm64Opcode::ARM64_SFT_INVALID)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    uint64_t result = a - b;
    bool n = (result & 0x8000000000000000) != 0;
    bool z = result == 0;
    bool c = a >= b; //if the result of a subtraction is positive or zero
    bool v = ((((a ^ b) & (a ^ result)) >> 20) & 0x80000000) != 0;
    Reg.SetConditionFlags(n, z, c, v);
}

void Arm64Op::Ldr(CPUExecutor & core, const Arm64Opcode &op)
{
    if ((op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM))
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

        if (op.WriteBack())
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

void Arm64Op::Mov(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG)
    {
        if (op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg) && op.Operand(1).shift.type == Arm64Opcode::ARM64_SFT_INVALID)
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

void Arm64Op::Sub(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is64bitReg(op.Operand(0).Reg) && CRegisters::Is64bitReg(op.Operand(1).Reg))
    {
        uint64_t a = 0, b = 0;
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_REG && CRegisters::Is64bitReg(op.Operand(2).Reg) && op.Operand(2).shift.type == Arm64Opcode::ARM64_SFT_INVALID && op.Operand(2).Extend == Arm64Opcode::ARM64_EXT_INVALID)
        {
            a = Reg.Get64(op.Operand(1).Reg);
            b = Reg.Get64(op.Operand(2).Reg);
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
        if (CRegisters::Is32bitReg(op.Operand(0).Reg) && op.Operand(1).type == Arm64Opcode::ARM64_OP_IMM)
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

void Arm64Op::Mul(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();

    if (op.Operands() == 3 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_REG && op.Operand(2).type == Arm64Opcode::ARM64_OP_REG &&
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg) && CRegisters::Is32bitReg(op.Operand(2).Reg))
    {
        Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) * Reg.Get32(op.Operand(2).Reg));
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
        CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
    {
        if (op.Operand(2).type == Arm64Opcode::ARM64_OP_IMM)
        {
            Reg.Set32(op.Operand(0).Reg, Reg.Get32(op.Operand(1).Reg) | (uint32_t)op.Operand(2).ImmVal);
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

        if (CRegisters::Is32bitReg(op.Operand(0).Reg) && CRegisters::Is32bitReg(op.Operand(1).Reg))
        {
            MMU.Write32(target_addr, Reg.Get32(op.Operand(0).Reg));
            MMU.Write32(target_addr + 4, Reg.Get32(op.Operand(1).Reg));
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

void Arm64Op::Str(CPUExecutor & core, const Arm64Opcode &op)
{
    CRegisters & Reg = core.Reg();
    MemoryManagement & MMU = core.MMU();

    if (op.Operands() == 2 && op.Operand(0).type == Arm64Opcode::ARM64_OP_REG && op.Operand(1).type == Arm64Opcode::ARM64_OP_MEM)
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

        if (CRegisters::Is32bitReg(op.Operand(0).Reg))
        {
            MMU.Write32(target_addr, Reg.Get32(op.Operand(0).Reg));
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
        
        if (CRegisters::Is32bitReg(op.Operand(0).Reg))
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

