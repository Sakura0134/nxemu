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

void CBufferQueue::SetPreallocatedBuffer(uint32_t slot, const IGBPBuffer& igbp_buffer)
{
    CGuard Guard(m_CS);

    Buffer buffer = {0};
    buffer.Slot = slot;
    buffer.IgbpBuffer = igbp_buffer;
    buffer.Status = BufferQueueStatus_Free;

    m_Queue.emplace_back(buffer);
    m_WaitEvent->GetKEventPtr()->Signal();
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
        Buffer& buffer = m_Queue[i];
        if (buffer.Status != BufferQueueStatus_Dequeued)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        buffer.Status = BufferQueueStatus_Queued;
        buffer.Transform = Transform;
        buffer.CropRect = CropRect;
        buffer.SwapInterval = SwapInterval;
        buffer.MultiFence = MultiFence;
        m_QueueSequence.push_back(Slot);
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}
