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
    DefaultString(MENU_OPEN_GAME, "&Open Game...");
    DefaultString(MENU_RECENT_GAMES, "Recent Games");
    DefaultString(MENU_EXIT, "E&xit");
    DefaultString(MENU_DEBUG, "Debug");
    DefaultString(MENU_DEBUG_COMMANDS_VIEW, "Commands...");
    DefaultString(MENU_DEBUG_LOGGING, "Logging...");

	//Options Menu
	DefaultString(MENU_OPTIONS, "&Options");
	DefaultString(MENU_SWITCH_KEYS, "Switch &Keys...");

	DefaultString(MSG_SELECT_GAME_DIR, "Select directory to load");
	DefaultString(MSG_LOADING, "Loading");
	DefaultString(MSG_LOADED_XCI, "Finished loading xci");
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