#include <nxemu-core\hle\Display\Nvidia\NvEvents.h>
#include <nxemu-core\hle\Kernel\KEvent.h>
#include <nxemu-core\SystemGlobals.h>

CNvEvents::CNvEvents()
{
    for (size_t i = 0, n = (sizeof(m_Registered) / sizeof(m_Registered[0])); i < n; i++)
    {
        m_Registered[i] = false;
        m_State[i] = StateFree;
    }
    for (size_t i = 0, n = MaxEvents; i < n; i++)
    {
        m_Event[i] = CKernelObjectPtr(new KEvent);
    }
}

CNvEvents::~CNvEvents()
{
}

bool CNvEvents::IsRegistered(uint32_t EventId) const
{
    return m_Registered[EventId];
}

void CNvEvents::Register(uint32_t EventId)
{
    m_Registered[EventId] = true;
    if (m_State[EventId] == StateFree) 
    {
        m_State[EventId] = StateRegistered;
    }
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
