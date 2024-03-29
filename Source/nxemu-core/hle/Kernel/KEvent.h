#pragma once
#include <Common\SyncEvent.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <stdint.h>

class KEvent :
    public CKernelObject
{
public:
    KEvent();

    void Clear();
    void Signal();
    bool ShouldWait(void) const;
    bool Wait(int64_t NanoSeconds);

private:
    KEvent(const KEvent&);            // Disable copy constructor
    KEvent& operator=(const KEvent&); // Disable assignment

    KernelObjectHandleType GetHandleType() const { return KernelObjectHandleType_Event; }
    KEvent * GetKEventPtr(void) { return this; }

    SyncEvent m_Event;
};

