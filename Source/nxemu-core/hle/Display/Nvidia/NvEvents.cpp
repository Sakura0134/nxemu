#include <nxemu-core\hle\Display\Nvidia\NvEvents.h>
#include <nxemu-core\hle\Kernel\KEvent.h>
#include <nxemu-core\SystemGlobals.h>

CNvEvents::CNvEvents()
{
    for (size_t i = 0, n = MaxEvents; i < n; i++)
    {
        m_Event[i] = CKernelObjectPtr(new KEvent);
    }
}

CNvEvents::~CNvEvents()
{
}

CKernelObjectPtr CNvEvents::GetEvent(uint32_t EventId)
{
    if (EventId < MaxEvents)
    {
        return m_Event[EventId];
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return CKernelObjectPtr();
}
