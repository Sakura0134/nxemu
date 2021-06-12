#pragma once
#include "VideoMemoryManager.h"
#include "GpuThread.h"
#include <nxemu-plugin-spec\Video.h>
#include <Common\CriticalSection.h>

class CVideo :
    public IVideo
{
    enum 
    {
        MaxSyncPoints = 192,
    };

public:
    CVideo(IRenderWindow& RenderWindow, ISwitchSystem& SwitchSystem);
    ~CVideo();

    //IVideo
    bool Initialize(void);
    void PushGPUEntries(const uint64_t* Entries, uint32_t NoOfEntries);
    void SwapBuffers(uint64_t Address, uint32_t Offset, uint32_t Format, uint32_t Width, uint32_t Height, uint32_t Stride, uint32_t Transform, int32_t CropLeft, int32_t CropTop, int32_t CropRight, int32_t CropBottom);
    uint32_t GetSyncPointValue(uint32_t SyncPointId) const;
    void WaitFence(uint32_t SyncPointId, uint32_t value);
    void FlushRegion(uint64_t CpuAddr, uint64_t size);
    void InvalidateRegion(uint64_t CpuAddr, uint64_t size);
    uint64_t VideoMemoryAllocate(uint64_t Size, uint64_t Align);
    uint64_t VideoMemoryAllocateFixed(uint64_t GpuAddr, uint64_t Size);
    void VideoMemoryMap(uint64_t CpuAddr, uint64_t GpuAddr, uint64_t size);
    uint64_t VideoMemoryMapAllocate(uint64_t CpuAddr, uint64_t size, uint64_t align);
private:
    CVideo();
    CVideo(const CVideo&);
    CVideo& operator=(const CVideo&);

    IRenderWindow & m_RenderWindow;
    ISwitchSystem & m_SwitchSystem;
    CVideoMemory m_Memory;
    uint32_t m_SyncPoints[MaxSyncPoints];
    mutable CriticalSection m_CS;
    CGpuThread m_GpuThread;
};