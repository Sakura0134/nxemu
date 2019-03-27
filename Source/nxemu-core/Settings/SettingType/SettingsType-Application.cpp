#include "SettingsType-Application.h"
#include <Common/path.h>
#include <Common/Trace.h>
#include <Common/StdString.h>
#include <nxemu-core/Settings/Settings.h>
#include <nxemu-core/SystemGlobals.h>

CIniFile * CSettingTypeApplication::m_SettingsIniFile = NULL;

CSettingTypeApplication::CSettingTypeApplication(const char * Section, const char * Name, uint32_t DefaultValue) :
    m_DefaultStr(""),
    m_DefaultValue(DefaultValue),
    m_DefaultSetting(Default_Constant),
    m_Section(FixSectionName(Section)),
    m_KeyName(Name),
    m_KeyNameIdex(m_KeyName)
{
}

CSettingTypeApplication::CSettingTypeApplication(const char * Section, const char * Name, bool DefaultValue) :
    m_DefaultStr(""),
    m_DefaultValue(DefaultValue),
    m_DefaultSetting(Default_Constant),
    m_Section(FixSectionName(Section)),
    m_KeyName(Name),
    m_KeyNameIdex(m_KeyName)
{
}

CSettingTypeApplication::CSettingTypeApplication(const char * Section, const char * Name, const char * DefaultValue) :
    m_DefaultStr(DefaultValue),
    m_DefaultValue(0),
    m_DefaultSetting(Default_Constant),
    m_Section(FixSectionName(Section)),
    m_KeyName(Name),
    m_KeyNameIdex(m_KeyName)
{
}

CSettingTypeApplication::CSettingTypeApplication(const char * Section, const char * Name, SettingID DefaultSetting) :
    m_DefaultStr(""),
    m_DefaultValue(0),
    m_DefaultSetting(DefaultSetting),
    m_Section(FixSectionName(Section)),
    m_KeyName(Name),
    m_KeyNameIdex(m_KeyName)
{
}

CSettingTypeApplication::~CSettingTypeApplication()
{
}

bool CSettingTypeApplication::IsSettingSet(void) const
{
    return m_SettingsIniFile ? m_SettingsIniFile->EntryExists(SectionName(), m_KeyNameIdex.c_str()) : false;
}

void CSettingTypeApplication::Initialize(void)
{
    CPath BaseDir(g_Settings->LoadStringVal(Cmd_BaseDirectory).c_str(), "");
    if (!BaseDir.DirectoryExists())
    {
        return;
    }

    stdstr SettingsFile, OrigSettingsFile;
    for (int i = 0; i < 100; i++)
    {
        OrigSettingsFile = SettingsFile;
        if (!g_Settings->LoadStringVal(SupportFile_Settings, SettingsFile) && i > 0)
        {
            break;
        }
        if (SettingsFile == OrigSettingsFile)
        {
            break;
        }
        if (m_SettingsIniFile)
        {
            delete m_SettingsIniFile;
        }
#ifdef _WIN32
        CPath SettingsDir(CPath(SettingsFile).GetDriveDirectory(), "");
#else
        CPath SettingsDir(CPath(SettingsFile).GetDirectory(), "");
#endif
        if (!SettingsDir.DirectoryExists())
        {
            SettingsDir.DirectoryCreate();
        }

        m_SettingsIniFile = new CIniFile(SettingsFile.c_str());
    }

    m_SettingsIniFile->SetAutoFlush(false);
}

void CSettingTypeApplication::Flush()
{
    if (m_SettingsIniFile)
    {
        m_SettingsIniFile->FlushChanges();
    }
}

void CSettingTypeApplication::ResetAll()
{
    if (m_SettingsIniFile == NULL)
    {
        return;
    }
    CIniFile::SectionList sections;
    m_SettingsIniFile->GetVectorOfSections(sections);
    for (size_t i = 0; i < sections.size(); i++)
    {
        m_SettingsIniFile->DeleteSection(sections[i].c_str());
    }
}

void CSettingTypeApplication::CleanUp()
{
    if (m_SettingsIniFile)
    {
        m_SettingsIniFile->SetAutoFlush(true);
        delete m_SettingsIniFile;
        m_SettingsIniFile = NULL;
    }
}

bool CSettingTypeApplication::Load(uint32_t /*Index*/, bool & Value) const
{
    bool bRes = false;

    uint32_t dwValue = 0;
    bRes = m_SettingsIniFile ? m_SettingsIniFile->GetNumber(SectionName(), m_KeyNameIdex.c_str(), Value, dwValue) : false;
    if (bRes)
    {
        Value = dwValue != 0;
    }

    if (!bRes && m_DefaultSetting != Default_None)
    {
        if (m_DefaultSetting == Default_Constant)
        {
            Value = m_DefaultValue != 0;
        }
        else
        {
            g_Settings->LoadBool(m_DefaultSetting, Value);
        }
    }
    return bRes;
}

bool CSettingTypeApplication::Load(uint32_t /*Index*/, uint32_t & Value) const
{
    bool bRes = m_SettingsIniFile->GetNumber(SectionName(), m_KeyNameIdex.c_str(), Value, Value);
    if (!bRes && m_DefaultSetting != Default_None)
    {
        Value = m_DefaultSetting == Default_Constant ? m_DefaultValue: g_Settings->LoadDword(m_DefaultSetting);
    }
    return bRes;
}

const char * CSettingTypeApplication::SectionName(void) const
{
    return m_Section.c_str();
}

bool CSettingTypeApplication::Load(uint32_t Index, std::string & Value) const
{
    bool bRes = m_SettingsIniFile ? m_SettingsIniFile->GetString(SectionName(), m_KeyNameIdex.c_str(), m_DefaultStr, Value) : false;
    if (!bRes)
    {
        CSettingTypeApplication::LoadDefault(Index, Value);
    }
    return bRes;
}

//return the default values
void CSettingTypeApplication::LoadDefault(uint32_t /*Index*/, bool & Value) const
{
    if (m_DefaultSetting != Default_None)
    {
        if (m_DefaultSetting == Default_Constant)
        {
            Value = m_DefaultValue != 0;
        }
        else
        {
            g_Settings->LoadBool(m_DefaultSetting, Value);
        }
    }
}

void CSettingTypeApplication::LoadDefault(uint32_t /*Index*/, uint32_t & Value) const
{
    if (m_DefaultSetting != Default_None)
    {
        Value = m_DefaultSetting == Default_Constant ? m_DefaultValue : g_Settings->LoadDword(m_DefaultSetting);
    }
}

void CSettingTypeApplication::LoadDefault(uint32_t /*Index*/, std::string & Value) const
{
    if (m_DefaultSetting != Default_None)
    {
        Value = m_DefaultSetting == Default_Constant ? m_DefaultStr : g_Settings->LoadStringVal(m_DefaultSetting);
    }
}

//Update the settings
void CSettingTypeApplication::Save(uint32_t /*Index*/, bool Value)
{
    if (m_DefaultSetting != Default_None &&
        ((m_DefaultSetting == Default_Constant && m_DefaultValue == (uint32_t)Value) ||
        (m_DefaultSetting != Default_Constant && g_Settings->LoadBool(m_DefaultSetting) == Value)))
    {
        m_SettingsIniFile->SaveString(SectionName(), m_KeyNameIdex.c_str(), NULL);
    }
    else
    {
        m_SettingsIniFile->SaveNumber(SectionName(), m_KeyNameIdex.c_str(), Value);
    }
}

void CSettingTypeApplication::Save(uint32_t /*Index*/, uint32_t Value)
{
    if (m_DefaultSetting != Default_None &&
        ((m_DefaultSetting == Default_Constant && m_DefaultValue == Value) ||
        (m_DefaultSetting != Default_Constant && g_Settings->LoadDword(m_DefaultSetting) == Value)))
    {
        m_SettingsIniFile->SaveString(SectionName(), m_KeyNameIdex.c_str(), NULL);
    }
    else
    {
        m_SettingsIniFile->SaveNumber(SectionName(), m_KeyNameIdex.c_str(), Value);
    }
}

void CSettingTypeApplication::Save(uint32_t Index, const std::string & Value)
{
    Save(Index, Value.c_str());
}

void CSettingTypeApplication::Save(uint32_t /*Index*/, const char * Value)
{
    if (m_DefaultSetting != Default_None &&
        ((m_DefaultSetting == Default_Constant && strcmp(m_DefaultStr,Value) == 0) ||
        (m_DefaultSetting != Default_Constant && strcmp(g_Settings->LoadStringVal(m_DefaultSetting).c_str(),Value) == 0)))
    {
        m_SettingsIniFile->SaveString(SectionName(), m_KeyNameIdex.c_str(), NULL);
    }
    else
    {
        m_SettingsIniFile->SaveString(SectionName(), m_KeyNameIdex.c_str(), Value);
    }
}

std::string CSettingTypeApplication::FixSectionName(const char * Section)
{
    stdstr SectionName(Section);

    if (SectionName.empty())
    {
        SectionName = "default";
    }
    SectionName.Replace("\\", "-");
    return SectionName;
}

void CSettingTypeApplication::Delete(uint32_t /*Index*/)
{
    m_SettingsIniFile->SaveString(SectionName(), m_KeyNameIdex.c_str(), NULL);
}