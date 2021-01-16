#pragma once
#include <Common\stdtypes.h>
#include <memory>

class CBufferQueue;

class CLayer
{
public:
    CLayer(uint32_t id, std::shared_ptr<CBufferQueue> buffer_queue);
    
private:
    CLayer(void);
    CLayer(const CLayer&);
    CLayer& operator=(const CLayer&);

    uint32_t m_id;
    std::shared_ptr<CBufferQueue> m_buffer_queue;
};