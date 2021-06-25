#pragma once
#include <stdint.h>

__interface IRenderer
{
    virtual bool Init(void) = 0;
    virtual void WaitForIdle(void) = 0;
};
