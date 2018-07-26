#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\Settings\SettingType\SettingsType-TempString.h>
#include <nxemu-core\Notification.h>

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

uint32_t CSettings::LoadDword(SettingID Type)
{
    uint32_t Value = 0;
    LoadDword(Type, Value);
    return Value;
}

bool CSettings::LoadDword(SettingID Type, uint32_t & Value)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
        return false;
    }
    if (FindInfo->second->IndexBasedSetting())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else
    {
        return FindInfo->second->Load(0, Value);
    }
    return false;
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

std::string CSettings::LoadStringIndex(SettingID Type, uint32_t index)
{
    std::string Value;
    LoadStringIndex(Type, index, Value);
    return Value;
}

bool CSettings::LoadStringIndex(SettingID Type, uint32_t index, std::string & Value)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
        return 0;
    }
    if (FindInfo->second->IndexBasedSetting())
    {
        return FindInfo->second->Load(index, Value);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return false;
}

void CSettings::SaveDword(SettingID Type, uint32_t Value)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
        return;
    }
    if (FindInfo->second->IndexBasedSetting())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else
    {
        FindInfo->second->Save(0, Value);
    }
}

void CSettings::SaveString(SettingID Type, const char * Value)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
    }
    FindInfo->second->Save(0, Value);
}

void CSettings::SaveStringIndex(SettingID Type, uint32_t index, const char * Buffer)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
    }
    if (FindInfo->second->IndexBasedSetting())
    {
        FindInfo->second->Save(index, Buffer);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CSettings::UnknownSetting(SettingID /*Type*/)
{
#ifdef _DEBUG
    g_Notify->BreakPoint(__FILE__, __LINE__);
#endif
}

