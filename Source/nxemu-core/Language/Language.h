#pragma once
#include "string.h"
#include "LanguageStringID.h"
#include <map>
#include <string>

class CLanguage
{
    typedef std::map<uint32_t, std::string, std::less<uint32_t> > LANG_STRINGS;

public:
    CLanguage();
    ~CLanguage();

    const std::string & GetString(LanguageStringID StringID);

private:
    CLanguage(const CLanguage&);				// Disable copy constructor
    CLanguage& operator=(const CLanguage&);		// Disable assignment

    LANG_STRINGS m_DefaultStrings;
    static const std::string m_emptyString;

    void DefaultString(LanguageStringID StringID, const char * string);
    void LoadDefaultStrings(void);
};

extern CLanguage * g_Lang;

inline const char * GS(LanguageStringID StringID)
{
    return g_Lang->GetString(StringID).c_str();
}

#ifdef _WIN32
const std::wstring wGS(LanguageStringID StringID);
#endif

extern CLanguage * g_Lang;
