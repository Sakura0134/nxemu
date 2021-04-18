#include <nxemu-core\hle\Kernel\KEvent.h>
#include <nxemu-core\hle\Display\Nvidia\BufferQueue.h>
#include <nxemu-core\SystemGlobals.h>

CBufferQueue::CBufferQueue(uint32_t Id, uint64_t LayerId) :
    m_Id(Id),
    m_LayerId(LayerId),
    m_WaitEvent(new KEvent)
{
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
