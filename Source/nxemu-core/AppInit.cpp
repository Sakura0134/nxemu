#include "AppInit.h"
#include <nxemu-core/Trace.h>
#include <nxemu-core/Settings.h>
#include <nxemu-core/SystemGlobals.h>
#include <nxemu-core/Language/Language.h>

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

static bool ParseCommand(int32_t argc, char **argv)
{
    if (argc <= 1)
    {
        return true;
    }
    for (int32_t i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            if (g_Settings->LoadStringVal(Cmd_RunFile).length() > 0)
            {
                WriteTrace(TraceAppInit, TraceError, "Run file has already been set");
                return false;
            }
            g_Settings->SaveString(Cmd_RunFile, &(argv[i][0]));
        }
        else
        {
            WriteTrace(TraceAppInit, TraceError, "unrecognized command-line parameter '%d: %s'", i, argv[i]);
            return false;
        }
    }
    return true;
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

    g_Lang = new CLanguage;

    WriteTrace(TraceAppInit, TraceDebug, "Parse Commands");
    if (!ParseCommand(argc, argv))
    {
        WriteTrace(TraceAppInit, TraceError, "Failed to Parse Commands, exiting now");
        return false;
    }

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