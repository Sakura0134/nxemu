#pragma once
#include <Common\stdtypes.h>
#include <string>

enum UISettingID
{
    Directory_RecentGameDirCount,
    Directory_RecentGameDirIndex,
};

void RegisterUISettings (void);

void UISettingsSaveDword(UISettingID Type, uint32_t Value);
void UISettingsSaveStringIndex(UISettingID Type, uint32_t index, const std::string & Value);

uint32_t UISettingsLoadDword(UISettingID Type);
bool UISettingsLoadDword(UISettingID Type, uint32_t & Value);
std::string UISettingsLoadStringIndex(UISettingID Type, uint32_t index);
