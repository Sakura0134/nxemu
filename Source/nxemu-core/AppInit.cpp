#include "AppInit.h"
#include <nxemu-core/Trace.h>
#include <nxemu-core/Settings.h>
#include <nxemu-core/SystemGlobals.h>

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

void InitializeLog(void)
{
#ifdef _DEBUG
    TraceSetMaxModule(MaxTraceModuleNXEmu, TraceInfo);
#else
    TraceSetMaxModule(MaxTraceModuleNXEmu, TraceError);
#endif
    SetTraceModuleNames();
}

bool AppInit(CNotification * Notify, const char * BaseDirectory, int argc, char **argv)
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

    g_Notify->AppInitDone();
    WriteTrace(TraceAppInit, TraceDebug, "Initialized Successfully");
    return true;
}

void AppCleanup(void)
{
}