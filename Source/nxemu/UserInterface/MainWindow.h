#pragma once
#include <string>
#include <nxemu\UserInterface\Debugger\Debugger.h>
#include "MainMenu.h"
#include "WTLApp.h"

class CMainGui :
    public CDebuggerUI
{
    enum
    {
        Width = 640,
        Height = 480,
    };

    enum { StatusBarID = 400 };

    BEGIN_MSG_MAP(CMainGui)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestory)
        COMMAND_ID_HANDLER(CMainMenu::ID_FILE_OPEN_GAME, OnOpenGame)
        COMMAND_ID_HANDLER(CMainMenu::ID_FILE_EXIT, OnFileExit)
        COMMAND_ID_HANDLER(CMainMenu::ID_OPTIONS_SWITCH_KEYS, OnSwitchKeys)
        COMMAND_ID_HANDLER(CMainMenu::ID_DEBUGGER_LOGGING, OnDebugLogging)
        COMMAND_RANGE_HANDLER(CMainMenu::ID_RECENT_GAME_START, CMainMenu::ID_RECENT_GAME_END, OnRecetGame)
    END_MSG_MAP()

public:
    CMainGui(const wchar_t * WindowTitle);
    ~CMainGui(void);

    WPARAM ProcessAllMessages(void);
    void ResetMenu();
    void Show(bool ShowWindow); //Show or Hide the current window
    void SetStatusText(int Panel, const wchar_t * Text);
	HWND hWnd(void) const { return m_hWnd; }

private:
    CMainGui(void);                       // Disable default constructor
    CMainGui(const CMainGui&);            // Disable copy constructor
    CMainGui& operator=(const CMainGui&); // Disable assignment

    void Create(void);
    bool RegisterWinClass(void);
    std::string ChooseFileToOpen(HWND hParent);
    void AddRecentGame(const char * ImagePath);

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnOpenGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnDebugLogging(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnSwitchKeys(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnRecetGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    static LRESULT __stdcall MainGui_Proc(HWND, UINT, WPARAM, LPARAM);

    static void GameFileChanged(CMainGui * Gui);
    static void GameNameChanged(CMainGui * Gui);

    CMainMenu m_Menu;

    HWND m_hWnd, m_hStatusWnd;
    std::wstring m_ClassName;
    uint32_t m_ThreadId;
    std::wstring m_WindowTitle;
};
