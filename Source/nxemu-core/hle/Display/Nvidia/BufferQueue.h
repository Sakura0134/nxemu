#pragma once
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <nxemu-core\hle\Display\Nvidia\NvMultiFence.h>
#include <Common\CriticalSection.h>
#include <Common\Padding.h>
#include <stdint.h>
#include <vector>
#include <list>

struct IGBPBuffer 
{
    uint32_t Magic;
    uint32_t Width;
    uint32_t Height;
    uint32_t Stride;
    uint32_t Format;
    uint32_t Usage;
    PADDING_WORDS(1);
    uint32_t Index;
    PADDING_WORDS(3);
    uint32_t gpu_buffer_id;
    PADDING_WORDS(17);
    uint32_t nvmap_handle;
    uint32_t offset;
    PADDING_WORDS(60);
};

enum BufferTransformFlags : uint32_t 
{
    BufferTransformFlags_NoTransform = 0x00,
    BufferTransformFlags_FlipH = 0x01,
    BufferTransformFlags_FlipV = 0x02,
    BufferTransformFlags_Rotate180 = 0x03,
    BufferTransformFlags_Rotate90 = 0x04,
    BufferTransformFlags_Rotate270 = 0x07,
};

enum BufferQueueStatus
{
    BufferQueueStatus_Free = 0,
    BufferQueueStatus_Queued = 1,
    BufferQueueStatus_Dequeued = 2,
    BufferQueueStatus_Acquired = 3,
};

enum BufferQueueQueryType
{
    BufferQueueQueryType_Width = 0,
    BufferQueueQueryType_Height = 1,
    BufferQueueQueryType_Format = 2,
};

class CBufferQueue
{
public:
    struct Rectangle
    {
        int32_t Left;
        int32_t Top;
        int32_t Right;
        int32_t Bottom;
    };

    struct Buffer
    {
        uint32_t Slot;
        BufferQueueStatus Status;
        IGBPBuffer IgbpBuffer;
        BufferTransformFlags Transform;
        Rectangle CropRect;
        uint32_t SwapInterval;
        NvMultiFence MultiFence;
    };

    CBufferQueue(uint32_t Id, uint64_t LayerId);
    uint32_t Query(BufferQueueQueryType Type);
    void QueueBuffer(uint32_t Slot, BufferTransformFlags Transform, const Rectangle & CropRect, uint32_t SwapInterval, const NvMultiFence & MultiFence);

    inline uint32_t Id(void) const { return m_Id; }
    inline CKernelObjectPtr WaitEvent(void) { return m_WaitEvent; }

private:
    CBufferQueue(void);
    CBufferQueue(const CBufferQueue&);
    CBufferQueue& operator=(const CBufferQueue&);

    typedef std::vector<Buffer> BufferList;

    CriticalSection m_CS;
    uint32_t m_Id;
    uint64_t m_LayerId;
    BufferList m_Queue;
    std::list<uint32_t> m_QueueSequence;
    CKernelObjectPtr m_WaitEvent;
};