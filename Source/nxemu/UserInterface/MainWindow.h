#pragma once
#include <string>
#include "MainMenu.h"
#include "WTLApp.h"

class CMainGui
{
    enum
    {
        Width = 640,
        Height = 480,
    };

    BEGIN_MSG_MAP(CMainGui)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestory)
        COMMAND_ID_HANDLER(CMainMenu::ID_FILE_OPEN_ROM, OnOpenRom)
        COMMAND_ID_HANDLER(CMainMenu::ID_FILE_EXIT, OnFileExit)
    END_MSG_MAP()

public:
    CMainGui(const wchar_t * WindowTitle);
    ~CMainGui(void);

    WPARAM ProcessAllMessages(void);
    void ResetMenu();
    void Show(bool ShowWindow); //Show or Hide the current window

private:
    CMainGui(void);                       // Disable default constructor
    CMainGui(const CMainGui&);            // Disable copy constructor
    CMainGui& operator=(const CMainGui&); // Disable assignment

    void Create(const wchar_t * WindowTitle);
    bool RegisterWinClass(void);
    std::string ChooseFileToOpen(HWND hParent);

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnOpenRom(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    
    static LRESULT __stdcall MainGui_Proc(HWND, UINT, WPARAM, LPARAM);

    CMainMenu m_Menu;

    HWND m_hWnd;
    std::wstring m_ClassName;
};
