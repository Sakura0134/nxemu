#include <nxemu-core\hle\Display\Nvidia\BufferQueue.h>
#include <nxemu-core\SystemGlobals.h>

CBufferQueue::CBufferQueue(uint32_t Id, uint64_t LayerId) :
    m_Id(Id),
    m_LayerId(LayerId)
{
}