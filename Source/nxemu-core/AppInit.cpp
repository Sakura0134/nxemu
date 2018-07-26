#include "AppInit.h"
#include <nxemu-core\Trace.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Language\Language.h>

void SetTraceModuleNames(void);

void InitializeLog(void)
{
#ifdef _DEBUG
    TraceSetMaxModule(MaxTraceModuleNXEmu, TraceInfo);
#else
    TraceSetMaxModule(MaxTraceModuleNXEmu, TraceError);
#endif
    SetTraceModuleNames();
}

void SetTraceModuleNames(void)
{
    TraceSetModuleName(TraceMD5, "MD5");
    TraceSetModuleName(TraceThread, "Thread");
    TraceSetModuleName(TracePath, "Path");
    TraceSetModuleName(TraceSettings, "Settings");
    TraceSetModuleName(TraceAppInit, "App Init");
    TraceSetModuleName(TraceAppCleanup, "App Cleanup");
    TraceSetModuleName(TraceUserInterface, "User Interface");
}

bool AppInit(CNotification * Notify, const char * BaseDirectory)
{
    g_Notify = Notify;
    InitializeLog();
    WriteTrace(TraceAppInit, TraceDebug, "Starting (BaseDirectory: %s)", BaseDirectory ? BaseDirectory : "null");
    if (Notify == NULL)
    {
        WriteTrace(TraceAppInit, TraceError, "No Notification class passed");
        return false;
    }

    WriteTrace(TraceAppInit, TraceDebug, "Settings up settings");
    g_Settings = new CSettings(BaseDirectory);

    g_Lang = new CLanguage;
    g_Notify->AppInitDone();
    WriteTrace(TraceAppInit, TraceDebug, "Initialized Successfully");
    return true;
}

void AppCleanup(void)
{
    if (g_Lang)
    {
        delete g_Lang;
        g_Lang = NULL;
    }
}