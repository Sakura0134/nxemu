#include <nxemu-core\hle\Kernel\KEvent.h>
#include <nxemu-core\hle\Display\Nvidia\BufferQueue.h>
#include <nxemu-core\SystemGlobals.h>

CBufferQueue::CBufferQueue(uint32_t Id, uint64_t LayerId) :
    m_Id(Id),
    m_LayerId(LayerId),
    m_WaitEvent(new KEvent)
{
}

uint32_t CBufferQueue::Query(BufferQueueQueryType Type)
{
    enum
    {
        FormatABGR8 = 1
    };

    switch (Type)
    {
    case BufferQueueQueryType_Format: return FormatABGR8;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

bool CBufferQueue::DequeueBuffer(uint32_t Width, uint32_t Height, uint32_t & Slot, NvMultiFence & MultiFence)
{
    CGuard Guard(m_CS);

    bool Found = false;
    for (size_t i = 0, n = m_Queue.size(); i < n; i++)
    {
        if (m_Queue[i].Status != BufferQueueStatus_Free)
        {
            continue;
        }
        if (m_Queue[i].IgbpBuffer.Width != Width)
        {
            continue;
        }
        if (m_Queue[i].IgbpBuffer.Height != Height)
        {
            continue;
        }
        m_Queue[i].Status = BufferQueueStatus_Dequeued;
        Slot = m_Queue[i].Slot;
        MultiFence = m_Queue[i].MultiFence;
        Found = true;
        break;
    }

    if (Found)
    {
        bool FreeQueue = false;
        for (size_t i = 0, n = m_Queue.size(); i < n; i++)
        {
            if (m_Queue[i].Status != BufferQueueStatus_Free)
            {
                continue;
            }
            FreeQueue = true;
            break;
        }

        if (!FreeQueue)
        {
            m_WaitEvent->GetKEventPtr()->Clear();
        }
    }
    return Found;
}

void CBufferQueue::SetPreallocatedBuffer(uint32_t slot, const IGBPBuffer& igbp_buffer)
{
    CGuard Guard(m_CS);

    BufferInfo Buffer = {0};
    Buffer.Slot = slot;
    Buffer.IgbpBuffer = igbp_buffer;
    Buffer.Status = BufferQueueStatus_Free;

    m_Queue.emplace_back(Buffer);
    m_WaitEvent->GetKEventPtr()->Signal();
}

const IGBPBuffer & CBufferQueue::RequestBuffer(uint32_t Slot)
{
    CGuard Guard(m_CS);

    for (size_t i = 0, n = m_Queue.size(); i < n; i++)
    {
        if (m_Queue[i].Slot != Slot)
        {
            continue;
        }
        if (m_Queue[i].Status != BufferQueueStatus_Dequeued)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        return m_Queue[i].IgbpBuffer;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    static IGBPBuffer empty = { 0 };
    return empty;
}

void CBufferQueue::QueueBuffer(uint32_t Slot, BufferTransformFlags Transform, const Rectangle & CropRect, uint32_t SwapInterval, const NvMultiFence & MultiFence)
{
    CGuard Guard(m_CS);

    for (size_t i = 0, n = m_Queue.size(); i < n; i++)
    {
        if (m_Queue[i].Slot != Slot)
        {
            continue;
        }
        BufferInfo & Buffer = m_Queue[i];
        if (Buffer.Status != BufferQueueStatus_Dequeued)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        Buffer.Status = BufferQueueStatus_Queued;
        Buffer.Transform = Transform;
        Buffer.CropRect = CropRect;
        Buffer.SwapInterval = SwapInterval;
        Buffer.MultiFence = MultiFence;
        m_QueueSequence.push_back(Slot);
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

bool CBufferQueue::AcquireBuffer(BufferInfo *& Buffer)
{
    CGuard Guard(m_CS);

    Buffer = nullptr;
    if (m_QueueSequence.size() != 0)
    {
        uint32_t Slot = m_QueueSequence.front();
        m_QueueSequence.pop_front();

        for (size_t i = 0, n = m_Queue.size(); i < n; i++)
        {
            BufferInfo & QueueBuffer = m_Queue[i];
            if (QueueBuffer.Status == BufferQueueStatus_Queued && QueueBuffer.Slot == Slot)
            {
                QueueBuffer.Status = BufferQueueStatus_Acquired;
                Buffer = &QueueBuffer;
                return true;
            }
        }
    }
    return false;
}

void CBufferQueue::ReleaseBuffer(uint32_t Slot)
{
    CGuard Guard(m_CS);

    bool Found = false;
    for (size_t i = 0, n = m_Queue.size(); i < n; i++)
    {
        BufferInfo & QueueBuffer = m_Queue[i];
        if (QueueBuffer.Slot != Slot)
        {
            continue;
        }
        if (QueueBuffer.Status != BufferQueueStatus_Acquired)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        QueueBuffer.Status = BufferQueueStatus_Free;
        Found = true;
        break;
    }
    if (!Found)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_WaitEvent->GetKEventPtr()->Signal();
}
