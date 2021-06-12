#include <nxemu-core\Machine\SystemTiming.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\hle\Display\Display.h>
#include <nxemu-core\hle\Display\Nvidia\NvDisp0.h>
#include <nxemu-core\SystemGlobals.h>

CDisplay::CDisplay(CSwitchSystem & System) :
    m_System(System),
    m_NvDriver(System.NvDriver()), 
    m_SystemEvents(System.SystemEvents()),
    m_NextLayerId(1),
    m_NextBufferQueueId(1),
    m_VsyncEvent(new KEvent),
    m_SwapInterval(1)
{
    m_DisplayList.push_back(std::make_shared<CDisplayDevice>(0, "Default"));
    m_SystemEvents.Schedule(GetNextTicks(), (CSystemEvents::ScheduleCallback)stVSyncEvent, (CSystemEvents::ScheduleParam)this);
}

CDisplay::~CDisplay()
{
    m_DisplayList.clear();
    m_BufferQueue.clear();
}

bool CDisplay::Open(const char * name, uint32_t & DisplayId)
{
    for (size_t i = 0, n = m_DisplayList.size(); i < n; i++)
    {
        if (strcmp(m_DisplayList[i]->Name(), name) != 0)
        {
            continue;
        }
        DisplayId = m_DisplayList[i]->Id();
        return true;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool CDisplay::CreateLayer(uint32_t DisplayId, uint32_t & LayerId)
{
    CDisplayDevice * display = FindDisplay(DisplayId);

    if (display == nullptr) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    LayerId = m_NextLayerId++;
    const uint32_t BufferQueueId = m_NextBufferQueueId++;
    std::shared_ptr<CBufferQueue> BufferQueue = std::make_shared<CBufferQueue>(BufferQueueId, LayerId);
    m_BufferQueue.push_back(BufferQueue);
    return display->CreateLayer(LayerId, BufferQueue);
}

bool CDisplay::FindBufferQueueId(uint32_t DisplayId, uint32_t LayerId, uint32_t & QueueId)
{
    CDisplayDevice * DisplayDevice = FindDisplay(DisplayId);
    if (DisplayDevice == nullptr)
    {
        return false;
    }
    CLayer * Layer = DisplayDevice->FindLayer(LayerId);
    if (Layer == nullptr)
    {
        return false;
    }
    QueueId = Layer->BufferQueue()->Id();
    return true;
}

CBufferQueue * CDisplay::FindBufferQueue(uint32_t QueueId)
{
    for (size_t i = 0, n = m_BufferQueue.size(); i < n; i++)
    {
        if (m_BufferQueue[i]->Id() == QueueId)
        {
            return m_BufferQueue[i].get();
        }
    }
    return nullptr;
}

void CDisplay::VSyncEvent(void)
{
    for (size_t i = 0, n = m_DisplayList.size(); i < n; i++)
    {
        CDisplayDevice& Device = *m_DisplayList[i];
        if (Device.Layers() == 0)
        {
            continue;
        }
        if (Device.Layers() > 1)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        CLayer & Layer = Device.Layer(0);
        CBufferQueue & BufferQueue = *(Layer.BufferQueue());
        CBufferQueue::BufferInfo * Buffer;
        if (!BufferQueue.AcquireBuffer(Buffer))
        {
            continue;
        }
        IGBPBuffer & IgbpBuffer = Buffer->IgbpBuffer;
        NvMultiFence & MultiFence = Buffer->MultiFence;
        for (uint32_t FenceIndx = 0; FenceIndx < MultiFence.NumFences; FenceIndx++) 
        {
            NvFence & Fence = MultiFence.fences[FenceIndx];
            m_System.Video().WaitFence(Fence.id, Fence.value);
        }
        CNvDisp0 & NvDisp0 = m_NvDriver.NvDisp0();
        NvDisp0.flip(IgbpBuffer.GPUBufferId, IgbpBuffer.Offset, IgbpBuffer.Format, IgbpBuffer.Width, IgbpBuffer.Height, IgbpBuffer.Stride, Buffer->Transform, Buffer->CropRect);
        m_SwapInterval = Buffer->SwapInterval;
        BufferQueue.ReleaseBuffer(Buffer->Slot);
    }
    m_SystemEvents.Schedule(GetNextTicks(), (CSystemEvents::ScheduleCallback)stVSyncEvent, (CSystemEvents::ScheduleParam)this);
}

int64_t CDisplay::GetNextTicks(void) const
{
    int64_t max_hertz = 120LL;
    return (CSystemTiming::BASE_CLOCK_RATE * (1LL << m_SwapInterval)) / max_hertz;
}

CDisplayDevice * CDisplay::FindDisplay(uint32_t DisplayId)
{
    for (size_t i = 0, n = m_DisplayList.size(); i < n; i++)
    {
        if (m_DisplayList[i]->Id() == DisplayId)
        {
            return m_DisplayList[i].get();
        }
    }
    return nullptr;
}
