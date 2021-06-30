#pragma once
#include <stdint.h>

__interface IRenderer
{
    virtual bool Init(void) = 0;
    virtual void FlushCommands(void) = 0;
    virtual void InvalidateRegion(uint64_t Addr, uint64_t Size) = 0;
    virtual void WaitForIdle(void) = 0;
    virtual void SignalSyncPoint(uint32_t Value) = 0;
};
