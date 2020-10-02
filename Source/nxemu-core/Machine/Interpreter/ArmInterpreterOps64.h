#pragma once
#include <nxemu-core\Machine\Arm64Opcode.h>
#include <nxemu-core\Machine\Interpreter\InterpreterCPU.h>

class Arm64Op
{
public:
    static void Add(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Adrp(CInterpreterCPU & core, const Arm64Opcode &op);
    static void And(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Asr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void B(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Bfi(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Bfxil(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Bic(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Bl(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Blr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Br(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Cbnz(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Cbz(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ccmp(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Cinc(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Clz(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Cmn(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Cmp(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Csel(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Cset(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Csetm(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Csinc(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Csinv(CInterpreterCPU& core, const Arm64Opcode& op);
    static void Dmb(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Dup(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Eor(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Fmov(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ins(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldarb(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldaxr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldp(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldrb(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldrh(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldrsb(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldrsw(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldur(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldurb(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ldurh(CInterpreterCPU& core, const Arm64Opcode& op);
    static void Ldxr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Lsl(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Lsr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Madd(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Mov(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Movi(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Movk(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Movn(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Movz(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Mrs(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Msr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Msub(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Mul(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Mvn(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Neg(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Orr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Rbit(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ret(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Sbfiz(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Sbfx(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Sdiv(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Smaddl(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Smulh(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Smull(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Stlr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Stlrb(CInterpreterCPU& core, const Arm64Opcode& op);
    static void Stlxr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Stp(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Str(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Strb(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Strh(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Stur(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Sturb(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Sturh(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Stxr(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Sub(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Svc(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Sxtw(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Tbnz(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Tbz(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Tst(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ubfiz(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Ubfx(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Udiv(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Umaddl(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Umulh(CInterpreterCPU & core, const Arm64Opcode &op);
    static void Umull(CInterpreterCPU & core, const Arm64Opcode &op);

private:
    static uint64_t MemIndex(const Arm64Opcode::MCOperand & Operand, IRegisters & Reg);
};