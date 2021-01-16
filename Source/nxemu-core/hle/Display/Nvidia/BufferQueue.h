#pragma once
#include <Common\stdtypes.h>

class CBufferQueue
{
public:
    CBufferQueue(uint32_t Id, uint64_t LayerId);

private:
    CBufferQueue(void);
    CBufferQueue(const CBufferQueue&);
    CBufferQueue& operator=(const CBufferQueue&);

    uint32_t m_Id;
    uint64_t m_LayerId;
};