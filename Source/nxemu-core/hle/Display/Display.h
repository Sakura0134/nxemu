#pragma once
#include <nxemu-core\hle\Display\DisplayDevice.h>
#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>
#include <nxemu-core\hle\Display\Nvidia\BufferQueue.h>
#include <nxemu-core\Machine\SystemEvents.h>
#include <stdint.h>
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
    bool FindBufferQueueId(uint32_t DisplayId, uint32_t LayerId, uint32_t & QueueId);
    CBufferQueue * FindBufferQueue(uint32_t QueueId);
    inline CKernelObjectPtr VsyncEvent(void) { return m_VsyncEvent; }

private:
    CDisplay(void);
    CDisplay(const CDisplay&);
    CDisplay& operator=(const CDisplay&);

    void VSyncEvent(void);
    int64_t GetNextTicks(void) const;
    CDisplayDevice * FindDisplay(uint32_t DisplayId);

    static void stVSyncEvent(CDisplay* _this) { _this->VSyncEvent(); }

    CSwitchSystem & m_System;
    CNvDriver & m_NvDriver;
    CSystemEvents & m_SystemEvents;
    DisplayList m_DisplayList;
    BufferQueues m_BufferQueue;
    uint32_t m_NextLayerId;
    uint32_t m_NextBufferQueueId;
    uint32_t m_SwapInterval;
    CKernelObjectPtr m_VsyncEvent;
};