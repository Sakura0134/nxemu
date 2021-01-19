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

bool KEvent::Wait(int64_t NanoSeconds)
{
    return m_Event.IsTriggered((int32_t)(NanoSeconds == -1 ? SyncEvent::INFINITE_TIMEOUT : NanoSeconds / 1000000));
}
