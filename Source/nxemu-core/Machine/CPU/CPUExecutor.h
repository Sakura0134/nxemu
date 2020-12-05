#pragma once
#include <nxemu-core\Machine\Registers.h>

__interface CPUExecutor
{
    IRegisters & Reg(void) = 0;
    void Execute(void) = 0;
};
