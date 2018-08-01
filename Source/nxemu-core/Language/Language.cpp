#include <nxemu-core/Language/Language.h>
#include <Common/StdString.h>

const std::string CLanguage::m_emptyString = "";

CLanguage::CLanguage()
{
    LoadDefaultStrings();
}

CLanguage::~CLanguage()
{
}

void CLanguage::LoadDefaultStrings(void)
{
    /*********************************************************************************
    * Menu                                                                           *
    *********************************************************************************/
    //File Menu
    DefaultString(MENU_FILE, "&File");
    DefaultString(MENU_LOAD_DIR, "&Load Dir...");
    DefaultString(MENU_RECENT_DIR, "Recent Dir");
    DefaultString(MENU_EXIT, "E&xit");

    DefaultString(MSG_SELECT_GAME_DIR, "Select directory to load");
}

void CLanguage::DefaultString(LanguageStringID StringID, const char * string)
{
    m_DefaultStrings.insert(LANG_STRINGS::value_type(StringID, string));
}

const std::string & CLanguage::GetString(LanguageStringID StringID)
{
    LANG_STRINGS::iterator DefString = m_DefaultStrings.find(StringID);
    if (DefString != m_DefaultStrings.end())
    {
        return DefString->second;
    }
#ifdef _DEBUG
    //g_Notify->BreakPoint(__FILE__, __LINE__);
#endif
    return m_emptyString;
}

#ifdef _WIN32
const std::wstring wGS(LanguageStringID StringID)
{
    return stdstr(g_Lang->GetString(StringID)).ToUTF16();
}
#endif