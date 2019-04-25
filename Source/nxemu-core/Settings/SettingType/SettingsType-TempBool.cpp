#include "SettingsType-TempBool.h"
#include <nxemu-core\SystemGlobals.h>

CSettingTypeTempBool::CSettingTypeTempBool(bool initialValue, const char * Name) :
    m_value(initialValue),
    m_changed(false),
    m_Name(Name ? Name : "")
{
}

CSettingTypeTempBool::~CSettingTypeTempBool(void)
{
}

bool CSettingTypeTempBool::Load(uint32_t /*Index*/, bool & Value) const
{
    Value = m_value;
    return true;
}

bool CSettingTypeTempBool::Load(uint32_t /*Index*/, uint32_t & Value) const
{
    Value = m_value ? 1 : 0;
    return true;
}

bool CSettingTypeTempBool::Load(uint32_t /*Index*/, std::string & /*Value*/) const
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

//return the default values
void CSettingTypeTempBool::LoadDefault(uint32_t /*Index*/, bool & /*Value*/) const
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CSettingTypeTempBool::LoadDefault(uint32_t /*Index*/, uint32_t & /*Value*/) const
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CSettingTypeTempBool::LoadDefault(uint32_t /*Index*/, std::string & /*Value*/) const
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CSettingTypeTempBool::Save(uint32_t /*Index*/, bool Value)
{
    m_value = Value;
    m_changed = true;
}

void CSettingTypeTempBool::Save(uint32_t /*Index*/, uint32_t /*Value*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CSettingTypeTempBool::Save(uint32_t /*Index*/, const std::string & /*Value*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CSettingTypeTempBool::Save(uint32_t /*Index*/, const char * /*Value*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CSettingTypeTempBool::Delete(uint32_t /*Index*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}