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

    //Plugins
    Plugin_Video_Selected,

    //Individual Game Settings
    Game_File,
    Game_Name,

    //Directory settings
    Directory_Plugin,
    Directory_PluginInitial,
    Directory_PluginSelected,
    Directory_PluginUseSelected,
    Directory_Log,
    Directory_LogInitial,
    Directory_LogSelected,
    Directory_LogUseSelected,

    //HLE Kernel
    HleKernel_RandomizeEntropy,
    HleKernel_RandomizeSeed,

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
    Debugger_TraceHleKernel,
    Debugger_TraceMemory,
    Debugger_TraceGameFile,

    FirstUISettings = MaxPluginSetting, LastUISettings = FirstUISettings + MaxPluginSetting,
};
