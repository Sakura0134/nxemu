#include "StateTracker.h"
#include "VideoNotification.h"

CStateTracker::CStateTracker(uint32_t NumRegisters, uint32_t MaxFlag) 
{
    if (MaxFlag > 0) 
    {
        m_Flags.resize(MaxFlag);
        m_MemoryWriteFlag.resize(MaxFlag);
        for (size_t i = 0, n = MaxFlag; i < n; i++)
        {
            m_Flags[i] = false;
            m_MemoryWriteFlag[i] = false;
        }
    }
    for (size_t i = 0, n = sizeof(m_RegisterFlag) / sizeof(m_RegisterFlag[0]); i < n; i++) 
    {
        m_RegisterFlag[i].resize(NumRegisters);
    }
}

void CStateTracker::FlagSet(uint32_t Index)
{
    if (Index >= m_Flags.size())
    {
        if (m_Flags.size() == 0)
        {
            return;
        }
        g_Notify->BreakPoint(__FILE__,__LINE__);
    }
    m_Flags[Index] = true;
}

void CStateTracker::RegisterChanged(uint32_t Method) 
{
    FlagSet(m_RegisterFlag[0][Method]);
    FlagSet(m_RegisterFlag[1][Method]);
}
