#pragma once
#include <Common/stdtypes.h>

class CDebugSettings
{
public:
    CDebugSettings();
    virtual ~CDebugSettings();

    static inline bool HaveDebugger(void) { return m_HaveDebugger; }

private:
    static void StaticRefreshSettings(CDebugSettings * _this)
    {
        _this->RefreshSettings();
    }

    void RefreshSettings(void);

    static bool m_HaveDebugger;

    static int32_t m_RefCount;
    static bool m_Registered;
};