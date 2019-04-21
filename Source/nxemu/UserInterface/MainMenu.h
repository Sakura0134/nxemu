#pragma once
#include <Common/CriticalSection.h>
#include <Windows.h>
#include "MenuItem.h"
#include <nxemu-core\Settings\Debug.h>

class CMainGui;

class CMainMenu :
    private CDebugSettings
{
public:
    enum MainMenuID
    {
        //File Menu
        ID_FILE_OPEN_GAME,
        ID_FILE_EXIT,

		//Options Menu
		ID_OPTIONS_SWITCH_KEYS,
		
		//Recent Game
        ID_RECENT_GAME_START,
        ID_RECENT_GAME_END = ID_RECENT_GAME_START + 20,

        ID_DEBUGGER_COMMANDS,
        ID_DEBUGGER_LOGGING,
    };

    CMainMenu(CMainGui * Window);
    ~CMainMenu();

    HMENU GetHandle(void) { return m_MenuHandle; }
    void ResetMenu(void);

private:
    void FillOutMenu(HMENU hMenu);

    static bool AddMenu(HMENU hMenu, const MenuItemList & Items);

    CMainGui * m_Gui;
    HMENU m_MenuHandle;
    CriticalSection m_CS;
};