#pragma once
#include <nxemu-core\Machine\Arm64Opcode.h>
#include <nxemu-core\Machine\CPU\CPUExecutor.h>

class Arm64Op
{
public:
    static void B(CPUExecutor & core, const Arm64Opcode &op);
    static void Cmp(CPUExecutor & core, const Arm64Opcode &op);
    static void Mov(CPUExecutor & core, const Arm64Opcode &op);
};