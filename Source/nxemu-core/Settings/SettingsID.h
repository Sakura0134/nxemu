#pragma once

enum
{
    MaxPluginSetting = 65535
};

enum SettingID
{
    //Default values
    Default_None,
    Default_Constant,

    //Command Settings
    Cmd_BaseDirectory,
    
    //Support Files
    SupportFile_Settings,
    SupportFile_SettingsDefault,
	SupportFile_Keys,
	SupportFile_KeysDefault,

    //Individual Game Settings
    Game_File,
    Game_Name,

    //Directory settings
    Directory_Log,
    Directory_LogInitial,
    Directory_LogSelected,
    Directory_LogUseSelected,

    //Debugger
    Debugger_Enabled,
    Debugger_SteppingOps,
    Debugger_WaitingForStep,

    //Logging
    Debugger_TraceAutoFlush,
    Debugger_TraceMD5,
    Debugger_TraceThread,
    Debugger_TracePath,
    Debugger_TraceSettings,
    Debugger_TraceAppInit,
    Debugger_TraceAppCleanup,
    Debugger_TraceServiceCall,
    Debugger_TraceGameFile,

    FirstUISettings = MaxPluginSetting, LastUISettings = FirstUISettings + MaxPluginSetting,
};
