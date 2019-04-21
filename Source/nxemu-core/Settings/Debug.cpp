#include "Settings.h"
#include <nxemu-core\SystemGlobals.h>
#include "Debug.h"

int32_t CDebugSettings::m_RefCount = 0;
bool CDebugSettings::m_Registered = false;

bool CDebugSettings::m_HaveDebugger = false;

CDebugSettings::CDebugSettings()
{
    m_RefCount += 1;
    if (!m_Registered && g_Settings)
    {
        m_Registered = true;
        g_Settings->RegisterChangeCB(Debugger_Enabled, this, (CSettings::SettingChangedFunc)StaticRefreshSettings);

        RefreshSettings();
    }
}

CDebugSettings::~CDebugSettings()
{
    m_RefCount -= 1;
    if (m_RefCount == 0 && g_Settings)
    {
        g_Settings->UnregisterChangeCB(Debugger_Enabled, this, (CSettings::SettingChangedFunc)StaticRefreshSettings);
    }
}

void CDebugSettings::RefreshSettings()
{
    m_HaveDebugger = g_Settings->LoadBool(Debugger_Enabled);
}