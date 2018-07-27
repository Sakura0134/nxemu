#pragma once
#include "SettingsType-Application.h"

class CSettingTypeApplicationPath :
    public CSettingTypeApplication
{
public:
    virtual ~CSettingTypeApplicationPath();

    CSettingTypeApplicationPath(const char * Section, const char * Name, SettingID DefaultSetting);
    bool IsSettingSet(void) const;

    //return the values
    virtual bool Load(uint32_t Index, std::string & Value) const;

private:
    CSettingTypeApplicationPath(void);                                            // Disable default constructor
    CSettingTypeApplicationPath(const CSettingTypeApplicationPath&);              // Disable copy constructor
    CSettingTypeApplicationPath& operator=(const CSettingTypeApplicationPath&);   // Disable assignment

    CSettingTypeApplicationPath(const char * Section, const char * Name, const char * DefaultValue);
    CSettingTypeApplicationPath(const char * Section, const char * Name, bool DefaultValue);
    CSettingTypeApplicationPath(const char * Section, const char * Name, uint32_t DefaultValue);
};
