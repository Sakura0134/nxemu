#include <Common\MemTest.h>
#include "AppInit.h"
#include <nxemu-core\Trace.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Language\Language.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <Common\path.h>
#include "Version.h"

void SetTraceModuleNames(void);

static CTraceFileLog * g_LogFile = NULL;

void InitializeLog(void)
{
#ifdef _DEBUG
    TraceSetMaxModule(MaxTraceModuleNXEmu, TraceInfo);
#else
    TraceSetMaxModule(MaxTraceModuleNXEmu, TraceError);
#endif
    SetTraceModuleNames();
}

void AddLogModule(void)
{
    CPath LogFilePath(g_Settings->LoadStringVal(Directory_Log).c_str(), "nxemu.log");
    if (!LogFilePath.DirectoryExists())
    {
        LogFilePath.DirectoryCreate();
    }

    g_LogFile = new CTraceFileLog(LogFilePath, g_Settings->LoadBool(Debugger_TraceAutoFlush), CLog::Log_New, 500);
    TraceAddModule(g_LogFile);
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
    TraceSetModuleName(TraceServiceCall, "Service Call");
    TraceSetModuleName(TraceHleKernel, "HLE Kernel");
    TraceSetModuleName(TraceMemory, "Memory");
    TraceSetModuleName(TraceGameFile, "Game File");
}

void UpdateTraceLevel(void)
{
    g_ModuleLogLevel[TraceMD5] = (uint8_t)g_Settings->LoadDword(Debugger_TraceMD5);
    g_ModuleLogLevel[TraceThread] = (uint8_t)g_Settings->LoadDword(Debugger_TraceThread);
    g_ModuleLogLevel[TracePath] = (uint8_t)g_Settings->LoadDword(Debugger_TracePath);
    g_ModuleLogLevel[TraceSettings] = (uint8_t)g_Settings->LoadDword(Debugger_TraceSettings);
    g_ModuleLogLevel[TraceAppInit] = (uint8_t)g_Settings->LoadDword(Debugger_TraceAppInit);
    g_ModuleLogLevel[TraceAppCleanup] = (uint8_t)g_Settings->LoadDword(Debugger_TraceAppCleanup);
    g_ModuleLogLevel[TraceServiceCall] = (uint8_t)g_Settings->LoadDword(Debugger_TraceServiceCall);
    g_ModuleLogLevel[TraceHleKernel] = (uint8_t)g_Settings->LoadDword(Debugger_TraceHleKernel);
    g_ModuleLogLevel[TraceMemory] = (uint8_t)g_Settings->LoadDword(Debugger_TraceMemory);
    g_ModuleLogLevel[TraceGameFile] = (uint8_t)g_Settings->LoadDword(Debugger_TraceGameFile);
}

void SetupTrace(void)
{
    AddLogModule();
    g_Settings->RegisterChangeCB(Debugger_TraceMD5, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->RegisterChangeCB(Debugger_TraceThread, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->RegisterChangeCB(Debugger_TracePath, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->RegisterChangeCB(Debugger_TraceSettings, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->RegisterChangeCB(Debugger_TraceAppInit, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->RegisterChangeCB(Debugger_TraceAppCleanup, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->RegisterChangeCB(Debugger_TraceServiceCall, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->RegisterChangeCB(Debugger_TraceHleKernel, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->RegisterChangeCB(Debugger_TraceMemory, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->RegisterChangeCB(Debugger_TraceGameFile, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    UpdateTraceLevel();
    WriteTrace(TraceAppInit, TraceInfo, "Application Starting %s", VER_FILE_VERSION_STR);
}

void CleanupTrace(void)
{
    g_Settings->UnregisterChangeCB(Debugger_TraceMD5, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->UnregisterChangeCB(Debugger_TraceThread, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->UnregisterChangeCB(Debugger_TracePath, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->UnregisterChangeCB(Debugger_TraceSettings, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->UnregisterChangeCB(Debugger_TraceAppInit, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->UnregisterChangeCB(Debugger_TraceAppCleanup, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->UnregisterChangeCB(Debugger_TraceServiceCall, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->UnregisterChangeCB(Debugger_TraceHleKernel, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->UnregisterChangeCB(Debugger_TraceMemory, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
    g_Settings->UnregisterChangeCB(Debugger_TraceGameFile, NULL, (CSettings::SettingChangedFunc)UpdateTraceLevel);
}

void TraceDone(void)
{
    CloseTrace();
    if (g_LogFile) { delete g_LogFile; g_LogFile = NULL; }
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

    g_Settings = new CSettings();
    if (!g_Settings->Initialize(BaseDirectory))
    {
        WriteTrace(TraceAppInit, TraceError, "Settings failed to initialize");
        return false;
    }

    SetupTrace();
    g_Lang = new CLanguage;
    g_Notify->AppInitDone();
    WriteTrace(TraceAppInit, TraceDebug, "Initialized Successfully");
    return true;
}

void AppCleanup(void)
{
    if (g_BaseMachine)
    {
        delete g_BaseMachine;
        g_BaseMachine = NULL;
    }
    if (g_Lang)
    {
        delete g_Lang;
        g_Lang = NULL;
    }
    CleanupTrace();
    if (g_Settings) 
    {
        delete g_Settings; 
        g_Settings = NULL; 
    }
    TraceDone();
}