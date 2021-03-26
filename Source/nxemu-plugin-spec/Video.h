#pragma once
#include <stdint.h>
#include "base.h"

__interface IVideo
{
    bool Initialize(void) = 0;
    void PushGPUEntries(const uint64_t * Entries, uint32_t NoOfEntries) = 0;
    void SwapBuffers(uint64_t Address, uint32_t Offset, uint32_t Format, uint32_t Width, uint32_t Height, uint32_t Stride, uint32_t Transform, int32_t CropLeft, int32_t CropTop, int32_t CropRight, int32_t CropBottom) = 0;
    uint32_t GetSyncPointValue(uint32_t SyncPointId) const = 0;
    void WaitFence(uint32_t SyncPointId, uint32_t value) = 0;
    void InvalidateRegion(uint64_t CpuAddr, uint64_t size) = 0;
    void FlushRegion(uint64_t CpuAddr, uint64_t size) = 0;
    uint64_t VideoMemoryAllocate(uint64_t Size, uint64_t Align) = 0;
    uint64_t VideoMemoryAllocateFixed(uint64_t VideoAddr, uint64_t Size) = 0;
    void VideoMemoryMap(uint64_t CpuAddr, uint64_t VideoAddr, uint64_t size) = 0;
    uint64_t VideoMemoryMapAllocate(uint64_t CpuAddr, uint64_t size, uint64_t align) = 0;
};

EXPORT IVideo * CALL CreateVideo(IRenderWindow & RenderWindow, ISwitchSystem & System);
EXPORT void CALL DestroyVideo(IVideo * Video);
