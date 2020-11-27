#pragma once
#include <nxemu-core/Language/Language.h>
#include <list>

class MenuItem;
typedef std::list<MenuItem> MenuItemList;

class MenuItem
{
public:
    enum Menu_ID
    {
        SPLITER = -1,
        SUB_MENU = -2,
        NO_ID = -3
    };
    static const std::wstring EMPTY_STDSTR;

    MenuItem(void);
    MenuItem(int ID, LanguageStringID Title = EMPTY_STRING, MenuItemList * SubMenu = nullptr, const std::wstring & ManualString = EMPTY_STDSTR);
    void Reset(int ID, LanguageStringID Title = EMPTY_STRING, MenuItemList * SubMenu = nullptr, const std::wstring & ManualString = EMPTY_STDSTR);

    int ID() const { return m_ID; }
    LanguageStringID Title() const { return m_Title; }
    bool ItemTicked() const { return m_ItemTicked; }
    bool ItemEnabled() const { return m_ItemEnabled; }
    const MenuItemList * SubMenu() const { return m_SubMenu; }
    const std::wstring & ManualString() const { return m_ManualString; }

    void SetItemTicked(bool ItemTicked) { m_ItemTicked = ItemTicked; }
    void SetItemEnabled(bool ItemEnabled) { m_ItemEnabled = ItemEnabled; }

private:

    int                m_ID;
    LanguageStringID   m_Title;
    MenuItemList *     m_SubMenu;
    std::wstring       m_ManualString;
    bool               m_ItemTicked;
    bool               m_ItemEnabled;
};

