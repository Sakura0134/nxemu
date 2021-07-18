#include "Decoder.h"
#include "Texture.h"
#include "VideoNotification.h"
#include <stdio.h>
#include <string.h>

uint32_t g_SwizzleTable[Texture_GOB_SizeY][Texture_GOB_SizeX];

void InitSwizzleTable(void) 
{
    for (uint32_t y = 0; y < Texture_GOB_SizeY; ++y)
    {
        for (uint32_t x = 0; x < Texture_GOB_SizeX; ++x) 
        {
            g_SwizzleTable[y][x] = ((x % 64) / 32) * 256 + ((y % 8) / 2) * 64 + ((x % 32) / 16) * 32 + (y % 2) * 16 + (x % 16);
        }
    }
}

void Swizzle(bool Linear, uint8_t * Output, uint64_t /*OutputSize*/, const uint8_t * Input, uint64_t /*InputSize*/, uint32_t BytesPerPixel, uint32_t Width, uint32_t Height, uint32_t Depth, uint32_t BlockHeight, uint32_t BlockDepth, uint32_t StrideAlignment) 
{
    uint32_t Pitch = Width * BytesPerPixel;
    uint32_t Stride = ((Width + ((1ULL << StrideAlignment) - 1)) >> StrideAlignment << StrideAlignment) * BytesPerPixel;
    uint32_t GobsInX = ((Stride + (1 << Texture_GOB_SizeXShift) - 1) >> Texture_GOB_SizeXShift);
    uint32_t BlockSize = GobsInX << (Texture_GOB_SizeShift + BlockHeight + BlockDepth);
    uint32_t SliceSize = ((Height + (1 << (BlockHeight + Texture_GOB_SizeYShift)) - 1) >> (BlockHeight + Texture_GOB_SizeYShift)) * BlockSize;

    uint32_t BlockHeightMask = (1U << BlockHeight) - 1;
    uint32_t BlockDepthMask = (1U << BlockDepth) - 1;
    uint32_t XShift = Texture_GOB_SizeShift + BlockHeight + BlockDepth;

    for (uint32_t z = 0; z < Depth; z++)
    {
        uint32_t OffsetZ = (z >> BlockDepth) * SliceSize + ((z & BlockDepthMask) << (Texture_GOB_SizeShift + BlockHeight));
        for (uint32_t y = 0; y < Height; y++) 
        {
            uint32_t(&table)[Texture_GOB_SizeX] = g_SwizzleTable[y % Texture_GOB_SizeY];
            uint32_t BlockY = y >> Texture_GOB_SizeYShift;
            uint32_t OffsetY = (BlockY >> BlockHeight) * BlockSize + ((BlockY & BlockHeightMask) << Texture_GOB_SizeShift);

            for (uint32_t column = 0; column < Width; column++)
            {
                uint32_t x = column * BytesPerPixel;
                uint32_t OffsetX = (x >> Texture_GOB_SizeXShift) << XShift;

                uint32_t BaseSwizzledOffset = OffsetZ + OffsetY + OffsetX;
                uint32_t SwizzledOffset = BaseSwizzledOffset + table[x % Texture_GOB_SizeX];
                uint32_t UnswizzledOffset = z * Pitch * Height + y * Pitch + column * BytesPerPixel;

                uint8_t * const Dst = &Output[Linear ? SwizzledOffset : UnswizzledOffset];
                const uint8_t * const Src = &Input[Linear ? UnswizzledOffset : SwizzledOffset];
                memcpy(Dst, Src, BytesPerPixel);
            }
        }
    }
}

void UnswizzleTexture(uint8_t * Output, uint64_t OutputSize, const uint8_t * Input, uint64_t InputSize, uint32_t BytesPerPixel, uint32_t Width, uint32_t Height, uint32_t Depth, uint32_t BlockHeight, uint32_t BlockDepth, uint32_t StrideAlignment)
{
    Swizzle(false, Output, OutputSize, Input, InputSize, BytesPerPixel, Width, Height, Depth, BlockHeight, BlockDepth, StrideAlignment);
}