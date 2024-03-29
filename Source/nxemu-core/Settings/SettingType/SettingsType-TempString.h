#pragma once

#include <nxemu-core/Settings/SettingType/SettingsType-Base.h>

class CSettingTypeTempString :
    public CSettingType
{
public:
    CSettingTypeTempString(const char * initialValue);
    ~CSettingTypeTempString();

    bool        IndexBasedSetting(void) const { return false; }
    SettingType GetSettingType(void) const { return SettingType_StringVariable; }
    bool        IsSettingSet(void) const { return false; }

    //return the values
    bool Load(uint32_t Index, bool & Value) const;
    bool Load(uint32_t Index, uint32_t & Value) const;
    bool Load(uint32_t Index, std::string & Value) const;

    //return the default values
    void LoadDefault(uint32_t Index, bool & Value) const;
    void LoadDefault(uint32_t Index, uint32_t & Value) const;
    void LoadDefault(uint32_t Index, std::string & Value) const;

    //Update the settings
    void Save(uint32_t Index, bool Value);
    void Save(uint32_t Index, uint32_t Value);
    void Save(uint32_t Index, const std::string & Value);
    void Save(uint32_t Index, const char * Value);

    // Delete the setting
    void Delete(uint32_t Index);

private:
    CSettingTypeTempString(void);                                     // Disable default constructor
    CSettingTypeTempString(const CSettingTypeTempString&);            // Disable copy constructor
    CSettingTypeTempString& operator=(const CSettingTypeTempString&); // Disable assignment

    std::string m_value;
};
