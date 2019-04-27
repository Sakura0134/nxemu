#include <nxemu-core\Machine\Interpreter\ArmInterpreterOps64.h>
#include <nxemu-core\SystemGlobals.h>

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
