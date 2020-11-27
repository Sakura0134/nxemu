#include "Notification.h"
#include <Common\StdString.h>
#include <nxemu\Settings\UISettings.h>
#include <nxemu\UserInterface\MainWindow.h>
#include <nxemu\UserInterface\SwitchKeysConfig.h>
#include <Windows.h>
#include <time.h>

CNotificationImp & Notify(void)
{
    static CNotificationImp Notify;
    return Notify;
}

CNotificationImp::CNotificationImp() :
    m_Gui(nullptr),
    m_NextMsg(0)
{
}

void CNotificationImp::DisplayError(const char * Message) const
{
    MessageBoxW(nullptr, stdstr(Message).ToUTF16().c_str(), L"Error", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
}

void CNotificationImp::DisplayMessage(uint32_t DisplayTime, LanguageStringID StringID) const
{
    DisplayMessage(DisplayTime, g_Lang->GetString(StringID).c_str());
}

void CNotificationImp::DisplayMessage(uint32_t DisplayTime, const char * Message) const
{
    if (m_Gui == nullptr) { return; }

    if (m_NextMsg > 0 || DisplayTime > 0)
    {
        time_t Now = time(nullptr);
        if (DisplayTime == 0 && Now < m_NextMsg)
        {
            return;
        }
        if (DisplayTime > 0)
        {
            m_NextMsg = Now + DisplayTime;
        }
        if (m_NextMsg == 0)
        {
            m_NextMsg = 0;
        }
    }
    m_Gui->SetStatusText(0, stdstr(Message).ToUTF16().c_str());
}

void CNotificationImp::BreakPoint(const char * FileName, uint32_t LineNumber)
{
    DisplayError(stdstr_f("Break point found at\n%s\n%d", FileName, LineNumber).c_str());
    if (IsDebuggerPresent() != 0)
    {
        DebugBreak();
    }
}

void CNotificationImp::AppInitDone(void)
{
    RegisterUISettings();
}

void CNotificationImp::SetMainWindow(CMainGui * Gui)
{
    m_Gui = Gui;
}

bool CNotificationImp::GetSwitchKeys(CSwitchKeys * keys) const
{
	CKeysConfig KeysConfig(keys);
	return KeysConfig.Display(m_Gui ? m_Gui->hWnd() : nullptr);
}
