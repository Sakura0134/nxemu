#include <nxemu-core/Settings.h>
#include <nxemu-core/Settings/SettingType/SettingsType-TempString.h>
#include <nxemu-core/Notification.h>

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

std::string CSettings::LoadStringVal(SettingID Type)
{
    std::string Value;
    LoadStringVal(Type, Value);
    return Value;
}

bool CSettings::LoadStringVal(SettingID Type, std::string & Value)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
        return 0;
    }

    return FindInfo->second->Load(0, Value);
}

bool CSettings::LoadStringVal(SettingID Type, char * Buffer, int BufferSize)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
        return 0;
    }
    bool bRes = false;
    if (FindInfo->second->IndexBasedSetting())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else
    {
        std::string Value;
        bRes = FindInfo->second->Load(0, Value);
        int len = BufferSize;
        if ((Value.length() + 1) < (size_t)len)
        {
            len = Value.length() + 1;
        }
        strncpy(Buffer, Value.c_str(), len);
    }
    return bRes;
}

void CSettings::SaveString(SettingID Type, const std::string & Value)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
        return;
    }
    FindInfo->second->Save(0, Value);
}

void CSettings::SaveString(SettingID Type, const char * Buffer)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
    }
    FindInfo->second->Save(0, Buffer);
}

void CSettings::UnknownSetting(SettingID /*Type*/)
{
#ifdef _DEBUG
    g_Notify->BreakPoint(__FILE__, __LINE__);
#endif
}

