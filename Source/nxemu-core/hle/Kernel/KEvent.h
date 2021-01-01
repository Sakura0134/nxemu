#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>

class KEvent :
    public CKernelObject
{
public:
    KEvent();

    void Signal();
    bool ShouldWait(void) const;

private:
    KEvent(const KEvent&);            // Disable copy constructor
    KEvent& operator=(const KEvent&); // Disable assignment

    KernelObjectHandleType GetHandleType() const { return KernelObjectHandleType_Event; }
    KEvent * GetKEventPtr(void) { return this; }

    bool m_Signaled;
};

