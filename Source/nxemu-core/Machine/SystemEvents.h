#pragma once
#include <Common\CriticalSection.h>
#include <stdint.h>
#include <vector>

class CInterpreterCPU;
class CSystemEvents
{
    friend CInterpreterCPU;
public:
    typedef size_t ScheduleParam;
    typedef void(*ScheduleCallback)(int64_t lpParam);

    CSystemEvents();
    ~CSystemEvents();
    
    void Schedule(int64_t Cycles, ScheduleCallback CallBack, ScheduleParam param);
    void TimerDone(void);

private:
    CSystemEvents(const CSystemEvents&);
    CSystemEvents& operator=(const CSystemEvents&);

    void CalcNextTimer(void);

    typedef struct
    {
        int64_t Cycles;
        ScheduleCallback CallBack;
        size_t param;
    } EventInfo;

    typedef std::vector<EventInfo> EventInfoList;

    CriticalSection m_CS;
    EventInfoList m_Events;
    int32_t m_NextTimer;
};