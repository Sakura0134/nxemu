#pragma once
#include <stdint.h>

uint32_t mullhi_u32(uint32_t x, uint32_t y);
uint64_t mullhi_u64(uint64_t x, uint64_t y);
int64_t mullhi_s64(int64_t x, int64_t y);
uint64_t mull128_u64(uint64_t x, uint64_t y, uint64_t * high);
uint64_t div128_to_64(uint64_t u1, uint64_t u0, uint64_t v, uint64_t* r);

uint32_t clz32(uint32_t val);
uint32_t clz64(uint64_t val);