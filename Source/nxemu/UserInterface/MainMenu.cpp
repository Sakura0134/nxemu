#include "MainMenu.h"
#include "MainWindow.h"
#include <Common\StdString.h>
#include <nxemu\Settings\UISettings.h>

CMainMenu::CMainMenu(CMainGui * hMainWindow) :
    m_Gui(hMainWindow),
    m_MenuHandle(NULL)
{
    ResetMenu();
}

CMainMenu::~CMainMenu()
{
}

void CMainMenu::ResetMenu(void)
{
    HMENU hMenu = CreateMenu();
    FillOutMenu(hMenu);

    HMENU OldMenuHandle;
    {
        CGuard Guard(m_CS);
        OldMenuHandle = m_MenuHandle;
        m_MenuHandle = hMenu;
    }
    m_Gui->ResetMenu();

    //Destroy the old menu
    DestroyMenu((HMENU)OldMenuHandle);
}

void CMainMenu::FillOutMenu(HMENU hMenu)
{
    CGuard Guard(m_CS);
    
    /* Recent Dir
    ****************/
    MenuItemList RecentDirMenu;
    uint32_t DirsToRemember = UISettingsLoadDword(Directory_RecentGameDirCount);
    for (uint32_t i = 0; i < DirsToRemember; i++)
    {
        std::string LastDir = UISettingsLoadStringIndex(Directory_RecentGameDirIndex, i);
        if (LastDir.empty())
        {
            break;
        }

        stdstr_f MenuString("&%d %s", i + 1, LastDir.c_str());
        RecentDirMenu.push_back(MenuItem(ID_RECENT_DIR_START + i, EMPTY_STRING, NULL, MenuString.ToUTF16(CP_ACP).c_str()));
    }

    MenuItemList FileMenu;
    FileMenu.push_back(MenuItem(ID_FILE_OPEN_ROM, MENU_OPEN_ROM));
    FileMenu.push_back(MenuItem(MenuItem::SPLITER));
    MenuItem RecentDirItem(MenuItem::SUB_MENU, MENU_RECENT_DIR, &RecentDirMenu, MenuItem::EMPTY_STDSTR);
    RecentDirItem.SetItemEnabled(RecentDirMenu.size() > 0);
    FileMenu.push_back(RecentDirItem);
    FileMenu.push_back(MenuItem(MenuItem::SPLITER));
    FileMenu.push_back(MenuItem(ID_FILE_EXIT, MENU_EXIT));

    MenuItemList MainTitleMenu;
    MainTitleMenu.push_back(MenuItem(MenuItem::SUB_MENU, MENU_FILE, &FileMenu));

    MenuItemList DebugMenu;
    DebugMenu.push_back(MenuItem(ID_DEBUGGER_LOGGING, MENU_DEBUG_LOGGING));
    MainTitleMenu.push_back(MenuItem(MenuItem::SUB_MENU, MENU_DEBUG, &DebugMenu));

    AddMenu(hMenu, MainTitleMenu);
}

bool CMainMenu::AddMenu(HMENU hMenu, const MenuItemList & Items)
{
    if (Items.begin() == Items.end()) { return false; }

    HMENU ItemID;
    UINT uFlags;
    std::wstring Text, String;
    for (MenuItemList::const_iterator MenuItem = Items.begin(); MenuItem != Items.end(); MenuItem++)
    {
        ItemID = (HMENU)((UINT_PTR)MenuItem->ID());
        uFlags = MF_STRING;
        Text = wGS(MenuItem->Title());

        if (MenuItem->Title() == EMPTY_STRING && MenuItem->ManualString().length() > 0)
        {
            Text = MenuItem->ManualString();
        }
        if (ItemID == (HMENU)MenuItem::SPLITER)
        {
            uFlags |= MF_SEPARATOR;
        }
        if (MenuItem->ItemTicked())
        {
            uFlags |= MFS_CHECKED;
        }
        if (MenuItem->ItemEnabled())
        {
            uFlags |= MFS_ENABLED;
        }
        else
        {
            uFlags |= MFS_DISABLED;
        }

        const MenuItemList * SubMenu = MenuItem->SubMenu();
        if (ItemID == (HMENU)MenuItem::SUB_MENU && HIWORD(SubMenu) != 0 && (SubMenu->begin() != SubMenu->end()))
        {
            ItemID = CreatePopupMenu();
            uFlags |= MF_POPUP;

            AddMenu(ItemID, *SubMenu);
        }

        AppendMenuW(hMenu, uFlags, (UINT_PTR)ItemID, Text.c_str());
    }
    return true;
}