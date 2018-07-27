#include "UISettings.h"
#include <nxemu-core\Settings\SettingsID.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\Settings\SettingType\SettingsType-Application.h>
#include <nxemu-core\Settings\SettingType\SettingsType-ApplicationIndex.h>
#include <nxemu-core\SystemGlobals.h>

void RegisterUISettings (void)
{
    g_Settings->AddHandler((SettingID)(FirstUISettings + Directory_RecentGameDirCount), new CSettingTypeApplication("", "Remembered Rom Dirs", (uint32_t)10));
    g_Settings->AddHandler((SettingID)(FirstUISettings + Directory_RecentGameDirIndex), new CSettingTypeApplicationIndex("Recent Dir", "Recent Dir", Default_None));
}

void UISettingsSaveDword(UISettingID Type, uint32_t Value)
{
    g_Settings->SaveDword((SettingID)(FirstUISettings + Type), Value);
}

void UISettingsSaveStringIndex(UISettingID Type, uint32_t index, const std::string & Value)
{
    g_Settings->SaveStringIndex((SettingID)(FirstUISettings + Type), index, Value.c_str());
}

uint32_t UISettingsLoadDword(UISettingID Type)
{
    return g_Settings->LoadDword((SettingID)(FirstUISettings + Type));
}

bool UISettingsLoadStringIndex(UISettingID Type, uint32_t index, std::string & Value)
{
    std::string ValueRes;
    bool res = g_Settings->LoadStringIndex((SettingID)(FirstUISettings + Type), index, ValueRes);
    Value = ValueRes;
    return res;
}

std::string UISettingsLoadStringIndex(UISettingID Type, uint32_t index)
{
    return g_Settings->LoadStringIndex((SettingID)(FirstUISettings + Type), index);
}

