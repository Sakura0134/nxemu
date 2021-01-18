#include <nxemu-core\hle\Kernel\KEvent.h>

KEvent::KEvent()
{
}

void KEvent::Clear()
{
    m_Event.Reset();
}

void KEvent::Signal()
{
    m_Event.Trigger();
}

bool KEvent::ShouldWait(void) const
{
    return m_Event.IsTriggered(0);
}
