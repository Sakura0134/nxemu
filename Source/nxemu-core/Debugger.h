#pragma once
#include <Common\stdtypes.h>

__interface CDebugger
{
    virtual void WaitForStep(void) = 0;
};