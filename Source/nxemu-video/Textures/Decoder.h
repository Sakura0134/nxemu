#pragma once
#include <stdint.h>

void InitSwizzleTable(void);
void UnswizzleTexture(uint8_t * Output, uint64_t OutputSize, const uint8_t * Input, uint64_t InputSize, uint32_t BytesPerPixel, uint32_t Width, uint32_t Height, uint32_t Depth, uint32_t BlockHeight, uint32_t BlockDepth, uint32_t StrideAlignment);
