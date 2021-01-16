#pragma once
#include <nxemu-core\hle\Display\DisplayDevice.h>
#include <nxemu-core\hle\Display\Nvidia\BufferQueue.h>
#include <Common\stdtypes.h>
#include <memory>
#include <vector>

class CSwitchSystem;

class CDisplay
{
    typedef std::vector<std::shared_ptr<CDisplayDevice>> DisplayList;
    typedef std::vector<std::shared_ptr<CBufferQueue>> BufferQueues;

public:
    CDisplay(CSwitchSystem & System);
    ~CDisplay();

    bool Open(const char * name, uint32_t & DisplayId);
    bool CreateLayer(uint32_t DisplayId, uint32_t & LayerId);

private:
    CDisplay(void);
    CDisplay(const CDisplay&);
    CDisplay& operator=(const CDisplay&);

    CDisplayDevice * FindDisplay(uint32_t DisplayId);
    CSwitchSystem & m_System;
    DisplayList m_DisplayList;
    BufferQueues m_BufferQueue;
    uint32_t m_NextLayerId;
    uint32_t m_NextBufferQueueId;
};