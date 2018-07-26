#pragma once
#include <Common\stdtypes.h>
#include <string>

enum UISettingID
{
    toadd
};

void RegisterUISettings (void);

void UISettingsSaveStringIndex(UISettingID Type, uint32_t index, const std::string & Value);

std::string UISettingsLoadStringIndex(UISettingID Type, uint32_t index);
