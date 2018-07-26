#pragma once

#include <nxemu-core\Settings\SettingType\SettingsType-Base.h>
#include <nxemu-core\Settings\SettingsID.h>
#include <map>

class CSettings
{
public:
    typedef void(*SettingChangedFunc)(void *);

    CSettings(const char * BaseDirectory);

    //return the values
    std::string LoadStringVal(SettingID Type);
    bool LoadStringVal(SettingID Type, std::string & Value);
    bool LoadStringVal(SettingID Type, char * Buffer, int32_t BufferSize);

    //Update the settings
    void SaveString(SettingID Type, const std::string & Value);
    void SaveString(SettingID Type, const char * Buffer);

private:
    CSettings(void);                          // Disable default constructor
    CSettings(const CSettings&);              // Disable copy constructor
    CSettings& operator=(const CSettings&);   // Disable assignment

    typedef std::map<SettingID, CSettingType *> SETTING_MAP;
    typedef SETTING_MAP::iterator SETTING_HANDLER;

    void AddHowToHandleSetting(const char * BaseDirectory);
    void AddHandler(SettingID TypeID, CSettingType * Handler);
    void UnknownSetting(SettingID Type);

    SETTING_MAP m_SettingInfo;
};
