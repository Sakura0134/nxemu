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

void CStateTracker::IncreaseMaxFlag(uint32_t MaxFlag) 
{
    size_t CurrentSize = m_Flags.size();
    if (MaxFlag < CurrentSize)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_Flags.resize(MaxFlag);
    m_MemoryWriteFlag.resize(MaxFlag);
    for (size_t i = CurrentSize, n = MaxFlag; i < n; i++) 
    {
        m_Flags[i] = false;
        m_MemoryWriteFlag[i] = false;
    }
}

void CStateTracker::OnMemoryWrite(void) 
{
    for (size_t i = 0, n = m_Flags.size(); i < n; i++)
    {
        if (!m_MemoryWriteFlag[i]) 
        {
            continue;
        }
        m_Flags[i] = true;
    }
}

void CStateTracker::MemoryWriteFlagSet(uint32_t Index) 
{
    m_MemoryWriteFlag[Index] = true;
}

void CStateTracker::FlagSetAll(void) 
{
    for (size_t i = 0, n = m_Flags.size(); i < n; i++) 
    {
        m_Flags[i] = true;
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

void CStateTracker::FlagClear(uint32_t Index)
{
    if (Index >= m_Flags.size())
    {
        g_Notify->BreakPoint(__FILE__,__LINE__);
    }
    m_Flags[Index] = false;
}

bool CStateTracker::Flag(uint32_t Index) const
{
    if (Index >= m_Flags.size())
    {
        g_Notify->BreakPoint(__FILE__,__LINE__);
    }
    return m_Flags[Index];
}

void CStateTracker::SetRegisterFlag(uint32_t Method, uint32_t Length, uint32_t Index) 
{
    if (Method + Length >= m_RegisterFlag[0].size()) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    for (uint32_t i = 0; i < Length; i++)
    {
        if (m_RegisterFlag[0][Method + i] == 0 || m_RegisterFlag[0][Method + i] == Index) 
        {
            m_RegisterFlag[0][Method + i] = Index;
        }
        else if (m_RegisterFlag[1][Method + i] == 0 || m_RegisterFlag[1][Method + i] == Index) 
        {
            m_RegisterFlag[1][Method + i] = Index;
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
}

void CStateTracker::RegisterChanged(uint32_t Method) 
{
    FlagSet(m_RegisterFlag[0][Method]);
    FlagSet(m_RegisterFlag[1][Method]);
}
