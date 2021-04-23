#pragma once
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <stdint.h>

class CNvEvents
{
public:
    enum
    {
        MaxEvents = 64
    };

    enum EventState
    {
        StateFree = 0,
        StateRegistered = 1,
        StateWaiting = 2,
        StateBusy = 3,
    };

    CNvEvents();
    ~CNvEvents();

    void Register(uint32_t EventId);
    bool IsRegistered(uint32_t EventId) const;
    CKernelObjectPtr GetEvent(uint32_t EventId);

private:
    CNvEvents(const CNvEvents&);
    CNvEvents& operator=(const CNvEvents&);

    bool m_Registered[MaxEvents];
    EventState m_State[MaxEvents];
    CKernelObjectPtr m_Event[MaxEvents];
};