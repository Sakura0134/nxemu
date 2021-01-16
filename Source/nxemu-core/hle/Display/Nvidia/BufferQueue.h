#pragma once
#include <Common\stdtypes.h>

class CBufferQueue
{
public:
    CBufferQueue(uint32_t Id, uint64_t LayerId);

    inline uint32_t Id(void) const { return m_Id; }

private:
    CBufferQueue(void);
    CBufferQueue(const CBufferQueue&);
    CBufferQueue& operator=(const CBufferQueue&);

    uint32_t m_Id;
    uint64_t m_LayerId;
};