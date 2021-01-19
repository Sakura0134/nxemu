#pragma once
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <Common/stdtypes.h>

class CNvEvents
{
public:
    enum
    {
        MaxEvents = 64
    };

    CNvEvents();
    ~CNvEvents();

    CKernelObjectPtr GetEvent(uint32_t EventId);

private:
    CNvEvents(const CNvEvents&);
    CNvEvents& operator=(const CNvEvents&);

    CKernelObjectPtr m_Event[MaxEvents];
};