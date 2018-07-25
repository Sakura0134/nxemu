#include "MenuItem.h"

const std::wstring MenuItem::EMPTY_STDSTR = L"";

MenuItem::MenuItem(void)
{
    Reset(NO_ID);
}

MenuItem::MenuItem(int ID, LanguageStringID Title, MenuItemList * SubMenu, const std::wstring & ManualString)
{
    Reset(ID, Title, SubMenu, ManualString);
}

void MenuItem::Reset(int ID, LanguageStringID Title, MenuItemList * SubMenu, const std::wstring & ManualString)
{
    m_ID = ID;
    m_Title = Title;
    m_SubMenu = SubMenu;
    m_ManualString = ManualString;
    m_ItemTicked = false;
    m_ItemEnabled = true;
}
