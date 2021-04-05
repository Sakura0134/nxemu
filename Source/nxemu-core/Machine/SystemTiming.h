#pragma once
#include <stdint.h>

class CSystemTiming
{
public:
    static uint64_t CpuCyclesToClockCycles(uint64_t ticks);

    static const uint64_t COUNT_FREQ;
    static const uint64_t BASE_CLOCK_RATE;
};
