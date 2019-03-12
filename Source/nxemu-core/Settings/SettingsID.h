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


    //Logging
    Debugger_TraceAppInit,
    Debugger_TraceAppCleanup,
    Debugger_TraceGameFile,
    FirstUISettings = MaxPluginSetting, LastUISettings = FirstUISettings + MaxPluginSetting,
};

