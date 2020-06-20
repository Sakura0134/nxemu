#include <nxemu-core\hle\Kernel\KEvent.h>

KEvent::KEvent() :
    m_Signaled(false)
{
}

void KEvent::Signal()
{
    m_Signaled = true;
}

bool KEvent::ShouldWait(void) const
{
    return !m_Signaled;
}
