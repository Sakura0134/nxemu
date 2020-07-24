#pragma once
#include <nxemu-core\Machine\Arm64Opcode.h>
#include <nxemu-core\Machine\CPU\CPUExecutor.h>

class Arm64Op
{
public:
    static void Add(CPUExecutor & core, const Arm64Opcode &op);
    static void Adrp(CPUExecutor & core, const Arm64Opcode &op);
    static void And(CPUExecutor & core, const Arm64Opcode &op);
    static void Asr(CPUExecutor & core, const Arm64Opcode &op);
    static void B(CPUExecutor & core, const Arm64Opcode &op);
    static void Bfi(CPUExecutor & core, const Arm64Opcode &op);
    static void Bic(CPUExecutor & core, const Arm64Opcode &op);
    static void Bl(CPUExecutor & core, const Arm64Opcode &op);
    static void Blr(CPUExecutor & core, const Arm64Opcode &op);
    static void Br(CPUExecutor & core, const Arm64Opcode &op);
    static void Cbnz(CPUExecutor & core, const Arm64Opcode &op);
    static void Cbz(CPUExecutor & core, const Arm64Opcode &op);
    static void Ccmp(CPUExecutor & core, const Arm64Opcode &op);
    static void Cinc(CPUExecutor & core, const Arm64Opcode &op);
    static void Clz(CPUExecutor & core, const Arm64Opcode &op);
    static void Cmn(CPUExecutor & core, const Arm64Opcode &op);
    static void Cmp(CPUExecutor & core, const Arm64Opcode &op);
    static void Csel(CPUExecutor & core, const Arm64Opcode &op);
    static void Cset(CPUExecutor & core, const Arm64Opcode &op);
    static void Csetm(CPUExecutor & core, const Arm64Opcode &op);
    static void Csinc(CPUExecutor & core, const Arm64Opcode &op);
    static void Csinv(CPUExecutor& core, const Arm64Opcode& op);
    static void Dmb(CPUExecutor & core, const Arm64Opcode &op);
    static void Dup(CPUExecutor & core, const Arm64Opcode &op);
    static void Eor(CPUExecutor & core, const Arm64Opcode &op);
    static void Fmov(CPUExecutor & core, const Arm64Opcode &op);
    static void Ins(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldarb(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldaxr(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldp(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldr(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldrb(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldrh(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldrsb(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldrsw(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldur(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldurb(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldurh(CPUExecutor& core, const Arm64Opcode& op);
    static void Ldxr(CPUExecutor & core, const Arm64Opcode &op);
    static void Lsl(CPUExecutor & core, const Arm64Opcode &op);
    static void Lsr(CPUExecutor & core, const Arm64Opcode &op);
    static void Madd(CPUExecutor & core, const Arm64Opcode &op);
    static void Mov(CPUExecutor & core, const Arm64Opcode &op);
    static void Movi(CPUExecutor & core, const Arm64Opcode &op);
    static void Movk(CPUExecutor & core, const Arm64Opcode &op);
    static void Movn(CPUExecutor & core, const Arm64Opcode &op);
    static void Movz(CPUExecutor & core, const Arm64Opcode &op);
    static void Mrs(CPUExecutor & core, const Arm64Opcode &op);
    static void Msr(CPUExecutor & core, const Arm64Opcode &op);
    static void Msub(CPUExecutor & core, const Arm64Opcode &op);
    static void Mul(CPUExecutor & core, const Arm64Opcode &op);
    static void Mvn(CPUExecutor & core, const Arm64Opcode &op);
    static void Neg(CPUExecutor & core, const Arm64Opcode &op);
    static void Orr(CPUExecutor & core, const Arm64Opcode &op);
    static void Rbit(CPUExecutor & core, const Arm64Opcode &op);
    static void Ret(CPUExecutor & core, const Arm64Opcode &op);
    static void Sbfiz(CPUExecutor & core, const Arm64Opcode &op);
    static void Sbfx(CPUExecutor & core, const Arm64Opcode &op);
    static void Smulh(CPUExecutor & core, const Arm64Opcode &op);
    static void Smull(CPUExecutor & core, const Arm64Opcode &op);
    static void Stlr(CPUExecutor & core, const Arm64Opcode &op);
    static void Stlrb(CPUExecutor& core, const Arm64Opcode& op);
    static void Stlxr(CPUExecutor & core, const Arm64Opcode &op);
    static void Stp(CPUExecutor & core, const Arm64Opcode &op);
    static void Str(CPUExecutor & core, const Arm64Opcode &op);
    static void Strb(CPUExecutor & core, const Arm64Opcode &op);
    static void Strh(CPUExecutor & core, const Arm64Opcode &op);
    static void Stur(CPUExecutor & core, const Arm64Opcode &op);
    static void Sturb(CPUExecutor & core, const Arm64Opcode &op);
    static void Sturh(CPUExecutor & core, const Arm64Opcode &op);
    static void Stxr(CPUExecutor & core, const Arm64Opcode &op);
    static void Sub(CPUExecutor & core, const Arm64Opcode &op);
    static void Svc(CPUExecutor & core, const Arm64Opcode &op);
    static void Sxtw(CPUExecutor & core, const Arm64Opcode &op);
    static void Tbnz(CPUExecutor & core, const Arm64Opcode &op);
    static void Tbz(CPUExecutor & core, const Arm64Opcode &op);
    static void Tst(CPUExecutor & core, const Arm64Opcode &op);
    static void Ubfiz(CPUExecutor & core, const Arm64Opcode &op);
    static void Ubfx(CPUExecutor & core, const Arm64Opcode &op);
    static void Udiv(CPUExecutor & core, const Arm64Opcode &op);
    static void Umaddl(CPUExecutor & core, const Arm64Opcode &op);
    static void Umulh(CPUExecutor & core, const Arm64Opcode &op);
    static void Umull(CPUExecutor & core, const Arm64Opcode &op);

private:
    static uint64_t MemIndex(const Arm64Opcode::MCOperand & Operand, CRegisters& Reg);
};