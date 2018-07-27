#include "Notification.h"
#include <Common\StdString.h>
#include <nxemu\Settings\UISettings.h>
#include <Windows.h>

CNotificationImp & Notify(void)
{
    static CNotificationImp Notify;
    return Notify;
}

CNotificationImp::CNotificationImp()
{
}

void CNotificationImp::DisplayError(const char * Message) const
{
    MessageBox(NULL, Message, "NXEmu - Error", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
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
