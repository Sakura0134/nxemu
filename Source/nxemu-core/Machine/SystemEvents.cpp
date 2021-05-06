#include <nxemu-core\Machine\SystemEvents.h>
#include <nxemu-core\SystemGlobals.h>

CSystemEvents::CSystemEvents() :
    m_NextTimer(0x7FFFFFFF)
{
}

CSystemEvents::~CSystemEvents()
{
}

void CSystemEvents::Schedule(int64_t Cycles, ScheduleCallback CallBack, ScheduleParam param)
{
    EventInfo Info;
    Info.Cycles = Cycles;
    Info.CallBack = CallBack;
    Info.param = param;

    CGuard Guard(m_CS);
    if (m_Events.size() != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_Events.push_back(Info);
    CalcNextTimer();
}

void CSystemEvents::CalcNextTimer(void)
{
    CGuard Guard(m_CS);
    int32_t NextTimer = 0x7FFFFFFF;

    for (EventInfoList::iterator itr = m_Events.begin(); itr != m_Events.end(); itr++)
    {
        EventInfo& Event = *itr;
        if (Event.Cycles >= NextTimer)
        {
            continue;
        }
        NextTimer = (int)Event.Cycles;
    }
    for (EventInfoList::iterator itr = m_Events.begin(); itr != m_Events.end(); itr++)
    {
        EventInfo& Event = *itr;
        Event.Cycles -= NextTimer;
    }
    m_NextTimer = NextTimer;
}
