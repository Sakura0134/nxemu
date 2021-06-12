#include "Video.h"
#include "VideoNotification.h"
#include <stdio.h>

CVideo::CVideo(IRenderWindow & RenderWindow, ISwitchSystem & SwitchSystem) :
    m_RenderWindow(RenderWindow),
    m_SwitchSystem(SwitchSystem),
    m_Memory(SwitchSystem)
{
}

CVideo::~CVideo()
{
}

bool CVideo::Initialize(void)
{
    return true;
}

void CVideo::PushGPUEntries(const uint64_t * /*Entries*/, uint32_t /*NoOfEntries*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CVideo::SwapBuffers(uint64_t /*Address*/, uint32_t /*Offset*/, uint32_t /*Format*/, uint32_t /*Width*/, uint32_t /*Height*/, uint32_t /*Stride*/, uint32_t /*Transform*/, int32_t /*CropLeft*/, int32_t /*CropTop*/, int32_t /*CropRight*/, int32_t /*CropBottom*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

uint32_t CVideo::GetSyncPointValue(uint32_t /*SyncPointId*/) const
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

void CVideo::WaitFence(uint32_t /*SyncPointId*/, uint32_t /*value*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CVideo::InvalidateRegion(uint64_t /*CpuAddr*/, uint64_t /*size*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CVideo::FlushRegion(uint64_t /*CpuAddr*/, uint64_t /*size*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

uint64_t CVideo::VideoMemoryAllocate(uint64_t Size, uint64_t Align)
{
    CGuard Guard(m_CS);
    return m_Memory.Allocate(Size, Align);
}

uint64_t CVideo::VideoMemoryAllocateFixed(uint64_t /*GpuAddr*/, uint64_t /*Size*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

void CVideo::VideoMemoryMap(uint64_t CpuAddr, uint64_t GpuAddr, uint64_t size)
{
    CGuard Guard(m_CS);
    m_Memory.Map(CpuAddr, GpuAddr, size);
}

uint64_t CVideo::VideoMemoryMapAllocate(uint64_t CpuAddr, uint64_t Size, uint64_t Align)
{
    CGuard Guard(m_CS);
    return m_Memory.MapAllocate(CpuAddr, Size, Align);
}