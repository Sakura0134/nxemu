#pragma once
#include <Common\stdtypes.h>
#include <string>

enum UISettingID
{
    //Recent Game
    File_RecentGameFileCount,
    File_RecentGameFileIndex,
};

void RegisterUISettings (void);

void UISettingsSaveDword(UISettingID Type, uint32_t Value);
void UISettingsSaveStringIndex(UISettingID Type, uint32_t index, const std::string & Value);

uint32_t UISettingsLoadDword(UISettingID Type);
std::string UISettingsLoadStringIndex(UISettingID Type, uint32_t index);
