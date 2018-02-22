#pragma once
#include <Common\stdtypes.h>
#include <string>

enum SettingType
{
    SettingType_Unknown = -1,
    SettingType_ConstString = 0,
    SettingType_ConstValue = 1,
    SettingType_StringVariable = 2,
};

class CSettingType
{
public:
    virtual ~CSettingType() {};

    virtual SettingType GetSettingType(void) const = 0;
    virtual bool IndexBasedSetting(void) const = 0;
    virtual bool IsSettingSet(void) const = 0;

    //return the values
    virtual bool Load(uint32_t Index, bool & Value) const = 0;
    virtual bool Load(uint32_t Index, uint32_t & Value) const = 0;
    virtual bool Load(uint32_t Index, std::string & Value) const = 0;

    //return the default values
    virtual void LoadDefault(uint32_t Index, bool & Value) const = 0;
    virtual void LoadDefault(uint32_t Index, uint32_t & Value) const = 0;
    virtual void LoadDefault(uint32_t Index, std::string & Value) const = 0;

    //Update the settings
    virtual void Save(uint32_t Index, bool Value) = 0;
    virtual void Save(uint32_t Index, uint32_t Value) = 0;
    virtual void Save(uint32_t Index, const std::string & Value) = 0;
    virtual void Save(uint32_t Index, const char * Value) = 0;

    // Delete the setting
    virtual void Delete(uint32_t Index) = 0;
};
