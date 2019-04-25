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

