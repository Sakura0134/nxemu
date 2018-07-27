#pragma once

#include <nxemu-core\Settings\SettingType\SettingsType-Base.h>
#include <nxemu-core\Settings\SettingsID.h>
#include <map>

class CSettings
{
public:
    typedef void(*SettingChangedFunc)(void *);

    CSettings(void);

    bool Initialize(const char * BaseDirectory);

    // read the settings
    uint32_t LoadDword(SettingID Type);
    bool LoadDword(SettingID Type, uint32_t & Value);
    std::string LoadStringVal(SettingID Type);
    bool LoadStringVal(SettingID Type, std::string & Value);
    std::string LoadStringIndex(SettingID Type, uint32_t index);
    bool LoadStringIndex(SettingID Type, uint32_t index, std::string & Value);

    //Update the settings
    void SaveDword(SettingID Type, uint32_t Value);
    void SaveString(SettingID Type, const char * Value);
    void SaveStringIndex(SettingID Type, uint32_t index, const char * Buffer);

    //Register Notification of change
    void RegisterChangeCB(SettingID Type, void * Data, SettingChangedFunc Func);
    void UnregisterChangeCB(SettingID Type, void * Data, SettingChangedFunc Func);

    void AddHandler(SettingID TypeID, CSettingType * Handler);

private:
    CSettings(const CSettings&);              // Disable copy constructor
    CSettings& operator=(const CSettings&);   // Disable assignment

    struct SETTING_CHANGED_CB
    {
        void * Data;
        SettingChangedFunc Func;
        SETTING_CHANGED_CB * Next;
    };

    typedef std::map<SettingID, SETTING_CHANGED_CB *> SETTING_CALLBACK;
    typedef std::map<SettingID, CSettingType *> SETTING_MAP;
    typedef SETTING_MAP::iterator SETTING_HANDLER;

    void AddHowToHandleSetting(const char * BaseDirectory);
    void UnknownSetting(SettingID Type);

    SETTING_MAP m_SettingInfo;
    SETTING_CALLBACK m_Callback;
};
