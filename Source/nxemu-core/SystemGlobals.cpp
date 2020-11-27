#include "SystemGlobals.h"
#include <nxemu-core\Notification.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\Language\Language.h>

CNotification * g_Notify = nullptr;
CLanguage * g_Lang = nullptr;
CSwitchSystem * g_BaseMachine = nullptr;
CSettings * g_Settings = nullptr;
CDebugger * g_Debugger = nullptr;
