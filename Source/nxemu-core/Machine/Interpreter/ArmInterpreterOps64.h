#pragma once
#include <nxemu-core\Machine\Arm64Opcode.h>
#include <nxemu-core\Machine\Interpreter\InterpreterCPU.h>

class Arm64Op
{
public:
    static void Add(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Adrp(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void And(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Asr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void B(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Bfi(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Bfxil(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Bic(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Bl(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Blr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Br(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Cbnz(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Cbz(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ccmp(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Cinc(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Clrex(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Clz(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Cmn(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Cmp(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Csel(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Cset(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Csetm(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Csinc(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Csinv(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Dmb(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Dsb(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Dup(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Eor(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Fmov(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ins(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldarb(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldaxr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldp(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldrb(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldrh(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldrsb(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldrsw(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldur(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldurb(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldurh(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ldxr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Lsl(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Lsr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Madd(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Mov(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Movi(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Movk(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Movn(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Movz(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Mrs(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Msr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Msub(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Mul(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Mvn(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Neg(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Orr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Rbit(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ret(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Sbfiz(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Sbfx(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Sdiv(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Smaddl(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Smulh(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Smull(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Stlr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Stlrb(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Stlxr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Stp(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Str(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Strb(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Strh(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Stur(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Sturb(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Sturh(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Stxr(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Sub(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Svc(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Sxtw(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Tbnz(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Tbz(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Tst(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ubfiz(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Ubfx(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Udiv(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Umaddl(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Umulh(CInterpreterCPU & Cpu, const Arm64Opcode & Op);
    static void Umull(CInterpreterCPU & Cpu, const Arm64Opcode & Op);

private:
    static uint64_t MemIndex(const Arm64Opcode::MCOperand & Operand, IRegisters & Reg);
};