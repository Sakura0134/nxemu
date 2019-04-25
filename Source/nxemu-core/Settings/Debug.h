#pragma once
#include <Common/stdtypes.h>

class CDebugSettings
{
public:
    CDebugSettings();
    virtual ~CDebugSettings();

    static inline const bool & HaveDebugger(void) { return m_HaveDebugger; }
    static inline const bool & isStepping(void) { return m_Stepping; }
    static inline const bool & WaitingForStep(void) { return m_WaitingForStep; }

private:
    static void StaticRefreshSettings(CDebugSettings * _this)
    {
        _this->RefreshSettings();
    }

    void RefreshSettings(void);

    static bool m_HaveDebugger;
    static bool m_Stepping;
    static bool m_WaitingForStep;

    static int32_t m_RefCount;
    static bool m_Registered;
};