#include "SystemGlobals.h"
#include <nxemu-core\Notification.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\Language\Language.h>

CNotification * g_Notify = NULL;
CLanguage * g_Lang = NULL;
CSwitchSystem * g_BaseMachine = NULL;
CSettings * g_Settings = NULL;
