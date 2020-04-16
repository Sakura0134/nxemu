#pragma once
#include <nxemu-core\Machine\Arm64Opcode.h>
#include <nxemu-core\Machine\CPU\CPUExecutor.h>

class Arm64Op
{
public:
    static void Add(CPUExecutor & core, const Arm64Opcode &op);
    static void And(CPUExecutor & core, const Arm64Opcode &op);
    static void B(CPUExecutor & core, const Arm64Opcode &op);
    static void Bfi(CPUExecutor & core, const Arm64Opcode &op);
    static void Bl(CPUExecutor & core, const Arm64Opcode &op);
    static void Cbz(CPUExecutor & core, const Arm64Opcode &op);
    static void Cmp(CPUExecutor & core, const Arm64Opcode &op);
    static void Dup(CPUExecutor & core, const Arm64Opcode &op);
    static void Ldr(CPUExecutor & core, const Arm64Opcode &op);
    static void Lsr(CPUExecutor & core, const Arm64Opcode &op);
    static void Mov(CPUExecutor & core, const Arm64Opcode &op);
    static void Movz(CPUExecutor & core, const Arm64Opcode &op);
    static void Mul(CPUExecutor & core, const Arm64Opcode &op);
    static void Neg(CPUExecutor & core, const Arm64Opcode &op);
    static void Orr(CPUExecutor & core, const Arm64Opcode &op);
    static void Stp(CPUExecutor & core, const Arm64Opcode &op);
    static void Str(CPUExecutor & core, const Arm64Opcode &op);
    static void Strb(CPUExecutor & core, const Arm64Opcode &op);
    static void Stur(CPUExecutor & core, const Arm64Opcode &op);
    static void Sturb(CPUExecutor & core, const Arm64Opcode &op);
    static void Sub(CPUExecutor & core, const Arm64Opcode &op);
    static void Sxtw(CPUExecutor & core, const Arm64Opcode &op);
};