#include <nxemu-core/Settings.h>
#include <nxemu-core/Settings/SettingType/SettingsType-TempString.h>

CSettings::CSettings(const char * BaseDirectory)
{
    AddHowToHandleSetting(BaseDirectory);
}

void CSettings::AddHowToHandleSetting(const char * BaseDirectory)
{
    AddHandler(Cmd_BaseDirectory, new CSettingTypeTempString(BaseDirectory));
}

void CSettings::AddHandler(SettingID TypeID, CSettingType * Handler)
{
    std::pair<SETTING_MAP::iterator, bool> res = m_SettingInfo.insert(SETTING_MAP::value_type(TypeID, Handler));
    if (!res.second)
    {
        delete res.first->second;
        m_SettingInfo.erase(res.first);
        res = m_SettingInfo.insert(SETTING_MAP::value_type(TypeID, Handler));
        if (!res.second)
        {
            delete Handler;
        }
    }
}

