#include "stdafx.h"

uint32_t mullhi_u32(uint32_t x, uint32_t y) 
{
    uint64_t xl = x, yl = y;
    uint64_t rl = xl * yl;
    return (uint32_t)(rl >> 32);
}

uint64_t mullhi_u64(uint64_t x, uint64_t y)
{
#if defined(_MSC_VER) && (defined(__x86_64__) || defined(_WIN64) || defined(_M_X64))
    return __umulh(x, y);
#elif defined(__SIZEOF_INT128__)
    __uint128_t xl = x, yl = y;
    __uint128_t rl = xl * yl;
    return (uint64_t)(rl >> 64);
#else
    // full 128 bits are x0 * y0 + (x0 * y1 << 32) + (x1 * y0 << 32) + (x1 * y1 << 64)
    uint32_t mask = 0xFFFFFFFF;
    uint32_t x0 = (uint32_t)(x & mask);
    uint32_t x1 = (uint32_t)(x >> 32);
    uint32_t y0 = (uint32_t)(y & mask);
    uint32_t y1 = (uint32_t)(y >> 32);
    uint32_t x0y0_hi = mullhi_u32(x0, y0);
    uint64_t x0y1 = x0 * (uint64_t)y1;
    uint64_t x1y0 = x1 * (uint64_t)y0;
    uint64_t x1y1 = x1 * (uint64_t)y1;
    uint64_t temp = x1y0 + x0y0_hi;
    uint64_t temp_lo = temp & mask;
    uint64_t temp_hi = temp >> 32;

    return x1y1 + temp_hi + ((temp_lo + x0y1) >> 32);
#endif
}

int64_t mullhi_s64(int64_t x, int64_t y)
{
#if defined(_MSC_VER) && (defined(__x86_64__) || defined(_WIN64) || defined(_M_X64))
    return __mulh(x, y);
#elif defined(__SIZEOF_INT128__)
    __int128_t xl = x, yl = y;
    __int128_t rl = xl * yl;
    return (int64_t)(rl >> 64);
#else
    // full 128 bits are x0 * y0 + (x0 * y1 << 32) + (x1 * y0 << 32) + (x1 * y1 << 64)
    uint32_t mask = 0xFFFFFFFF;
    uint32_t x0 = (uint32_t)(x & mask);
    uint32_t y0 = (uint32_t)(y & mask);
    int32_t x1 = (int32_t)(x >> 32);
    int32_t y1 = (int32_t)(y >> 32);
    uint32_t x0y0_hi = libdivide__mullhi_u32(x0, y0);
    int64_t t = x1 * (int64_t)y0 + x0y0_hi;
    int64_t w1 = x0 * (int64_t)y1 + (t & mask);

    return x1 * (int64_t)y1 + (t >> 32) + (w1 >> 32);
#endif
}

uint32_t clz32(uint32_t val)
{
#if defined(__GNUC__)
    return val ? __builtin_clz(val) : 32;
#else
    /* Binary search for the leading one bit.  */
    int cnt = 0;

    if ((val & 0xFFFF0000U) == 0) 
    {
        cnt += 16;
        val <<= 16;
    }
    if ((val & 0xFF000000U) == 0)
    {
        cnt += 8;
        val <<= 8;
    }
    if ((val & 0xF0000000U) == 0) 
    {
        cnt += 4;
        val <<= 4;
    }
    if ((val & 0xC0000000U) == 0) 
    {
        cnt += 2;
        val <<= 2;
    }
    if ((val & 0x80000000U) == 0) 
    {
        cnt++;
        val <<= 1;
    }
    if ((val & 0x80000000U) == 0) 
    {
        cnt++;
    }
    return cnt;
#endif
}

uint32_t clz64(uint64_t val)
{
#if defined(__GNUC__)
    return val ? __builtin_clzll(val) : 64;
#else
    int cnt = 0;

    if ((val >> 32) == 0) 
    {
        cnt += 32;
    }
    else 
    {
        val >>= 32;
    }

    return cnt + clz32((uint32_t)val);
#endif
}
