#pragma once
#include <stdint.h>

struct NvFence 
{
    int32_t id;
    uint32_t value;
};

struct NvMultiFence
{
    uint32_t NumFences;
    NvFence fences[4];
};