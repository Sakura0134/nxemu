#pragma once
#include <stdint.h>

__interface CDebugger
{
    virtual void WaitForStep(void) = 0;
};