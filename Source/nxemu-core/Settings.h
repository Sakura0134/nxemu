#pragma once

#include <nxemu-core/Settings/SettingType/SettingsType-Base.h>
#include <nxemu-core/Settings/SettingsID.h>
#include <map>

class CSettings
{
public:
    typedef void(*SettingChangedFunc)(void *);

    CSettings(const char * BaseDirectory);

private:
    CSettings(void);                          // Disable default constructor
    CSettings(const CSettings&);              // Disable copy constructor
    CSettings& operator=(const CSettings&);   // Disable assignment

    typedef std::map<SettingID, CSettingType *> SETTING_MAP;
    typedef SETTING_MAP::iterator SETTING_HANDLER;

    void AddHowToHandleSetting(const char * BaseDirectory);
    void AddHandler(SettingID TypeID, CSettingType * Handler);

    SETTING_MAP m_SettingInfo;
};
