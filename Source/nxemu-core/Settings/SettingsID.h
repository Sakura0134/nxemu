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

    FirstUISettings = MaxPluginSetting, LastUISettings = FirstUISettings + MaxPluginSetting,
};

