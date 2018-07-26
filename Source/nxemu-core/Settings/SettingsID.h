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

    FirstUISettings = MaxPluginSetting, LastUISettings = FirstUISettings + MaxPluginSetting,
};

