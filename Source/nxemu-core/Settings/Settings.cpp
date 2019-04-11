#include <Common\MemTest.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <nxemu-core\Settings\SettingType\SettingsType-Application.h>
#include <nxemu-core\Settings\SettingType\SettingsType-ApplicationPath.h>
#include <nxemu-core\Settings\SettingType\SettingsType-RelativePath.h>
#include "SettingType/SettingsType-SelectedDirectory.h"
#include <nxemu-core\Settings\SettingType\SettingsType-TempString.h>
#include <nxemu-core\Settings\Settings.h>

CSettings::CSettings()
{
}

CSettings::~CSettings()
{
    CSettingTypeApplication::CleanUp();

    for (SETTING_MAP::iterator iter = m_SettingInfo.begin(); iter != m_SettingInfo.end(); iter++)
    {
        delete iter->second;
    }

    for (SETTING_CALLBACK::iterator cb_iter = m_Callback.begin(); cb_iter != m_Callback.end(); cb_iter++)
    {
        SETTING_CHANGED_CB * item = cb_iter->second;
        while (item != NULL)
        {
            SETTING_CHANGED_CB * current_item = item;
            item = item->Next;
            delete current_item;
        }
    }
}

bool CSettings::Initialize(const char * BaseDirectory)
{
    AddHowToHandleSetting(BaseDirectory);
    CSettingTypeApplication::Initialize();
    return true;
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

void CSettings::AddHowToHandleSetting(const char * BaseDirectory)
{
    AddHandler(Cmd_BaseDirectory, new CSettingTypeTempString(BaseDirectory));

    //Support Files
    AddHandler(SupportFile_Settings, new CSettingTypeApplicationPath("Settings", "ConfigFile", SupportFile_SettingsDefault));
    AddHandler(SupportFile_SettingsDefault, new CSettingTypeRelativePath("Config", "NXEmu.cfg"));
	AddHandler(SupportFile_Keys, new CSettingTypeApplicationPath("Settings", "KeyFile", SupportFile_KeysDefault));
	AddHandler(SupportFile_KeysDefault, new CSettingTypeRelativePath("Config", "switch.keys"));

    //Individual Game Settings
    AddHandler(Game_File, new CSettingTypeTempString(""));
    AddHandler(Game_Name, new CSettingTypeTempString(""));

    //Directory settings
    AddHandler(Directory_Log, new CSettingTypeSelectedDirectory("Dir:Log", Directory_LogInitial, Directory_LogSelected, Directory_LogUseSelected, Directory_Log));
    AddHandler(Directory_LogInitial, new CSettingTypeRelativePath("Logs", ""));
    AddHandler(Directory_LogSelected, new CSettingTypeApplicationPath("Log Directory", "Directory", Directory_LogInitial));
    AddHandler(Directory_LogUseSelected, new CSettingTypeApplication("Log Directory", "Use Selected", false));

    //Logging
    AddHandler(Debugger_TraceAutoFlush, new CSettingTypeApplication("Logging", "Trace Auto Flush", (uint32_t)false));
    AddHandler(Debugger_TraceMD5, new CSettingTypeApplication("Logging", "MD5", (uint32_t)g_ModuleLogLevel[TraceMD5]));
    AddHandler(Debugger_TraceThread, new CSettingTypeApplication("Logging", "Thread", (uint32_t)g_ModuleLogLevel[TraceThread]));
    AddHandler(Debugger_TracePath, new CSettingTypeApplication("Logging", "Path", (uint32_t)g_ModuleLogLevel[TracePath]));
    AddHandler(Debugger_TraceSettings, new CSettingTypeApplication("Logging", "Settings", (uint32_t)g_ModuleLogLevel[TraceSettings]));
    AddHandler(Debugger_TraceAppInit, new CSettingTypeApplication("Logging", "App Init", (uint32_t)g_ModuleLogLevel[TraceAppInit]));
    AddHandler(Debugger_TraceAppCleanup, new CSettingTypeApplication("Logging", "App Cleanup", (uint32_t)g_ModuleLogLevel[TraceAppCleanup]));
    AddHandler(Debugger_TraceGameFile, new CSettingTypeApplication("Logging", "Game File", (uint32_t)g_ModuleLogLevel[TraceGameFile]));
}

bool CSettings::LoadBool(SettingID Type)
{
    bool Value = false;
    LoadBool(Type, Value);
    return Value;
}

bool CSettings::LoadBool(SettingID Type, bool & Value)
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
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else
    {
        return FindInfo->second->Load(0, Value);
    }
    return false;
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

void CSettings::SaveBool(SettingID Type, bool Value)
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
    NotifyCallBacks(Type);
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
    NotifyCallBacks(Type);
}

void CSettings::SaveString(SettingID Type, const char * Value)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        //if not found do nothing
        UnknownSetting(Type);
    }
    else if (FindInfo->second->IndexBasedSetting())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else
    {
        FindInfo->second->Save(0, Value);
    }
    NotifyCallBacks(Type);
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
    NotifyCallBacks(Type);
}

bool CSettings::IsSettingSet(SettingID Type)
{
    SETTING_HANDLER FindInfo = m_SettingInfo.find(Type);
    if (FindInfo == m_SettingInfo.end())
    {
        return false;
    }
    return FindInfo->second->IsSettingSet();
}

void CSettings::RegisterChangeCB(SettingID Type, void * Data, SettingChangedFunc Func)
{
    SETTING_CHANGED_CB * new_item = new SETTING_CHANGED_CB;
    new_item->Data = Data;
    new_item->Func = Func;
    new_item->Next = NULL;

    SETTING_CALLBACK::iterator Callback = m_Callback.find(Type);
    if (Callback != m_Callback.end())
    {
        SETTING_CHANGED_CB * item = Callback->second;
        while (item->Next)
        {
            item = item->Next;
        }
        item->Next = new_item;
    }
    else
    {
        m_Callback.insert(SETTING_CALLBACK::value_type(Type, new_item));
    }
}

void CSettings::UnregisterChangeCB(SettingID Type, void * Data, SettingChangedFunc Func)
{
    bool bRemoved = false;

    //Find out the information for handling the setting type from the list
    SETTING_CALLBACK::iterator Callback = m_Callback.find(Type);
    if (Callback != m_Callback.end())
    {
        SETTING_CHANGED_CB * PrevItem = NULL;
        SETTING_CHANGED_CB * item = Callback->second;

        while (item)
        {
            if (Callback->first == Type && item->Data == Data && item->Func == Func)
            {
                bRemoved = true;
                if (PrevItem == NULL)
                {
                    if (item->Next)
                    {
                        SettingID CallbackType = Callback->first;
                        SETTING_CHANGED_CB * Next = item->Next;
                        m_Callback.erase(Callback);
                        m_Callback.insert(SETTING_CALLBACK::value_type(CallbackType, Next));
                    }
                    else
                    {
                        m_Callback.erase(Callback);
                    }
                }
                else
                {
                    PrevItem->Next = item->Next;
                }
                delete item;
                item = NULL;
                break;
            }
            PrevItem = item;
            item = item->Next;
        }
    }
    else
    {
        UnknownSetting(Type);
        return;
    }

    if (!bRemoved)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CSettings::NotifyCallBacks(SettingID Type)
{
    SETTING_CALLBACK::iterator Callback = m_Callback.find(Type);
    if (Callback == m_Callback.end())
    {
        return;
    }

    for (SETTING_CHANGED_CB * item = Callback->second; item != NULL; item = item->Next)
    {
        item->Func(item->Data);
    }
}

void CSettings::UnknownSetting(SettingID /*Type*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}
