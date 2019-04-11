#include <nxemu-core\FileFormat\nacp.h>

NACP::NACP(void) :
    m_Info({0})
{
}

bool NACP::Load(CEncryptedFile * EncryptedFile, uint64_t Offset, uint64_t FileOffset, uint64_t FileSize)
{
    if (EncryptedFile == NULL)
    {
        return false;
    }
    if (FileSize < sizeof(m_Info))
    {
        return false;
    }
    if (EncryptedFile->Seek(Offset + FileOffset, CFileBase::begin) < 0)
    {
        return false;
    }
    if (EncryptedFile->Read(&m_Info, sizeof(m_Info), FileOffset) != sizeof(m_Info))
    {
        return false;
    }
    return true;
}

std::string NACP::GetApplicationName(Language language) const
{
    const LanguageEntry& entry = GetLanguageEntry(language);
    uint32_t len = 0, max_len = sizeof(LanguageEntry::application_name) / sizeof(LanguageEntry::application_name[0]);
    while (len < max_len)
    {
        if (entry.application_name[len] == '\0')
        {
            break;
        }
        len += 1;
    }
    return std::string((const char *)&entry.application_name[0], len);
}

const NACP::LanguageEntry& NACP::GetLanguageEntry(Language language) const 
{
    if (language != Language::Default) 
    {
        return m_Info.language_entries[language];
    }

    for (uint32_t i = 0, n = sizeof(NACPInfo::language_entries) / sizeof(NACPInfo::language_entries[0]); i < n; i++)
    {
        const LanguageEntry &entry = m_Info.language_entries[i];
        if (entry.application_name[0] != '\0')
        {
            return entry;
        }
    }

    return GetLanguageEntry(Language::AmericanEnglish);
}

