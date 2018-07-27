#pragma once
#include <Common/CriticalSection.h>
#include <Windows.h>
#include "MenuItem.h"

class CMainGui;

class CMainMenu 
{
public:
    enum MainMenuID
    {
        //File Menu
        ID_FILE_LOAD_DIR,
        ID_FILE_EXIT,

        //Recent Dir
        ID_RECENT_DIR_START,
        ID_RECENT_DIR_END = ID_RECENT_DIR_START + 20,
    };

    CMainMenu(CMainGui * Window);
    ~CMainMenu();

    HMENU GetHandle(void) { return m_MenuHandle; }

private:
    void ResetMenu(void);
    void FillOutMenu(HMENU hMenu);

    static bool AddMenu(HMENU hMenu, const MenuItemList & Items);

    CMainGui * m_Gui;
    HMENU m_MenuHandle;
    CriticalSection m_CS;
};