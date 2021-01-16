#include <nxemu-core\hle\Display\Layer.h>

CLayer::CLayer(uint32_t id, std::shared_ptr<CBufferQueue> buffer_queue) :
    m_id(id),
    m_buffer_queue(buffer_queue)
{
}
