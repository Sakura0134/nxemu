#include <nxemu-core\Machine\SystemTiming.h>
#include <Common\Maths.h>

const uint64_t CSystemTiming::BASE_CLOCK_RATE = 1019215872; // Switch clock speed - 1020MHz
const uint64_t CSystemTiming::COUNT_FREQ = 19200000;

uint64_t CSystemTiming::CpuCyclesToClockCycles(uint64_t ticks)
{
    uint64_t hi, rem;
    uint64_t lo = mull128_u64(ticks, COUNT_FREQ, &hi);
    return div128_to_64(hi, lo, BASE_CLOCK_RATE, &rem);
}