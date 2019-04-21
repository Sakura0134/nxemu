#include <nxemu-core\Machine\SwitchRom.h>
#include <nxemu-core\Settings\SettingType\SettingsType-Application.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu\Settings\UISettings.h>
#include <nxemu\UserInterface\MainWindow.h>
#include <nxemu\UserInterface\SwitchKeysConfig.h>
#include <Common\path.h>
#include <Common\StdString.h>
#include <Windows.h>

CMainGui::CMainGui(const wchar_t * WindowTitle) :
    m_hWnd(NULL),
    m_hStatusWnd(NULL),
    m_ClassName(L"NXEmu"),
    m_Menu(this),
    m_ThreadId(GetCurrentThreadId()),
    m_WindowTitle(WindowTitle)
{
    g_Settings->RegisterChangeCB(Game_File, this, (CSettings::SettingChangedFunc)GameFileChanged);
    g_Settings->RegisterChangeCB(Game_Name, this, (CSettings::SettingChangedFunc)GameNameChanged);

    RegisterWinClass();
    Create();
}

CMainGui::~CMainGui()
{
    g_Settings->UnregisterChangeCB(Game_File, this, (CSettings::SettingChangedFunc)GameFileChanged);
    g_Settings->UnregisterChangeCB(Game_Name, this, (CSettings::SettingChangedFunc)GameNameChanged);
    if (m_hWnd)
    {
        DestroyWindow((HWND)m_hWnd);
    }
}

bool CMainGui::RegisterWinClass(void)
{
    WNDCLASSW wcl;

    wcl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wcl.cbClsExtra = 0;
    wcl.cbWndExtra = 0;
    wcl.hIcon = NULL;
    wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcl.hInstance = GetModuleHandle(NULL);

    wcl.lpfnWndProc = (WNDPROC)MainGui_Proc;
    wcl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcl.lpszMenuName = NULL;
    wcl.lpszClassName = m_ClassName.c_str();
    if (RegisterClassW(&wcl) == 0) return false;
    return true;
}

std::string CMainGui::ChooseFileToOpen(HWND hParent)
{
    CPath FileName;
    const char * Filter = "Switch Files (*.xci)\0*.xci\0All files (*.*)\0*.*\0";
    if (FileName.SelectFile(hParent, CPath(CPath::MODULE_DIRECTORY), Filter, true))
    {
        return FileName;
    }
    return "";
}

void CMainGui::AddRecentGame(const char * ImagePath)
{
    if (HIWORD(ImagePath) == NULL) { return; }

    //Get Information about the stored rom list
    size_t MaxRememberedFiles = UISettingsLoadDword(File_RecentGameFileCount);

    strlist RecentGames;
    for (uint32_t i = 0; i < MaxRememberedFiles; i++)
    {
        stdstr RecentGame = UISettingsLoadStringIndex(File_RecentGameFileIndex, i);
        if (RecentGame.empty())
        {
            break;
        }
        RecentGames.push_back(RecentGame);
    }

    //See if the dir is already in the list if so then move it to the top of the list
    for (strlist::iterator iter = RecentGames.begin(); iter != RecentGames.end(); iter++)
    {
        if (_stricmp(ImagePath, iter->c_str()) != 0)
        {
            continue;
        }
        RecentGames.erase(iter);
        break;
    }
    RecentGames.push_front(ImagePath);
    if (RecentGames.size() > MaxRememberedFiles)
    {
        RecentGames.pop_back();
    }

    uint32_t i = 0;
    for (strlist::iterator iter = RecentGames.begin(); iter != RecentGames.end(); iter++)
    {
        UISettingsSaveStringIndex(File_RecentGameFileIndex, i++, *iter);
    }
    m_Menu.ResetMenu();
}

void CMainGui::Create(void)
{
    CreateWindowExW(WS_EX_ACCEPTFILES, m_ClassName.c_str(), m_WindowTitle.c_str(), WS_OVERLAPPED | WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, Width, Height,
        NULL, NULL, GetModuleHandle(NULL), this);
}

WPARAM CMainGui::ProcessAllMessages(void)
{
    MSG msg;

    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

void CMainGui::Show(bool Visible)
{
    if (m_hWnd)
    {
        ShowWindow((HWND)m_hWnd, Visible ? SW_SHOW : SW_HIDE);
    }
}

LRESULT CMainGui::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ResetMenu();

    m_hStatusWnd = (HWND)CreateStatusWindow(WS_CHILD | WS_VISIBLE, "", m_hWnd, StatusBarID);
    SendMessage((HWND)m_hStatusWnd, SB_SETTEXT, 0, (LPARAM)"");

    RECT clrect, swrect;
    GetClientRect(m_hWnd, &clrect);
    GetClientRect((HWND)m_hStatusWnd, &swrect);

    int Parts[2];
    Parts[0] = (Width - (int)(clrect.right * 0.25));
    Parts[1] = Width;

    SendMessage((HWND)m_hStatusWnd, SB_SETPARTS, 2, (LPARAM)&Parts[0]);
    MoveWindow((HWND)m_hStatusWnd, 0, clrect.bottom - swrect.bottom, Width, Height, TRUE);

    int X = (GetSystemMetrics(SM_CXSCREEN) - Width) / 2;
    int	Y = (GetSystemMetrics(SM_CYSCREEN) - Height) / 2;

    SetWindowPos(m_hWnd, NULL, X, Y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    return 0;
}

LRESULT CMainGui::OnDestory(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    PostQuitMessage(0);
    return 0;
}

LRESULT CMainGui::OnOpenGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    std::string File = ChooseFileToOpen(m_hWnd);
    if (File.length() == 0)
    {
        return 0;
    }
    LaunchSwitchRom(File.c_str());
    return 0;
}

LRESULT CMainGui::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DestroyWindow(m_hWnd);
    return 0;
}

LRESULT CMainGui::OnDebugViewCommands(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    OpenCommandWindow();
    return 0;
}

LRESULT CMainGui::OnDebugLogging(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EditLoggingOptions(m_hWnd);
    return 0;
}

LRESULT CMainGui::OnSwitchKeys(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSwitchKeys SwitchKeys;
	CKeysConfig KeysConfig(&SwitchKeys);
	KeysConfig.Display(m_hWnd);
	return 0;
}

LRESULT CMainGui::OnRecetGame(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    std::string FileName = UISettingsLoadStringIndex(File_RecentGameFileIndex, LOWORD(wID) - CMainMenu::ID_RECENT_GAME_START);
    if (FileName.length() > 0)
    {
        LaunchSwitchRom(FileName.c_str());
    }
    return 0;
}

LRESULT CMainGui::MainGui_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, (uintptr_t)lpcs->lpCreateParams);
        CMainGui * _this = (CMainGui *)lpcs->lpCreateParams;
        _this->m_hWnd = hWnd;
    }
    CMainGui * _this = (CMainGui *)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (_this)
    {
        LRESULT result;
        if (_this->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, result))
        {
            return result;
        }
    }
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void CMainGui::ResetMenu()
{
    SetMenu(m_hWnd, m_Menu.GetHandle());
}

void CMainGui::SetStatusText(int Panel, const wchar_t * Text)
{
    static wchar_t Message[2][500];
    if (Panel >= 2)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    wchar_t * Msg = Message[Panel];

    memset(Msg, 0, sizeof(Message[0]));
    _snwprintf(Msg, sizeof(Message[0]) / sizeof(Message[0][0]), L"%s", Text);
    Msg[(sizeof(Message[0]) / sizeof(Message[0][0])) - 1] = 0;
    if (GetCurrentThreadId() == m_ThreadId)
    {
        SendMessageW((HWND)m_hStatusWnd, SB_SETTEXTW, Panel, (LPARAM)Msg);
    }
    else 
    {
        PostMessageW((HWND)m_hStatusWnd, SB_SETTEXTW, Panel, (LPARAM)Msg);
    }
}

void CMainGui::GameFileChanged(CMainGui * Gui)
{
    std::string FileLoc = g_Settings->LoadStringVal(Game_File);
    if (FileLoc.length() > 0)
    {
        Gui->AddRecentGame(FileLoc.c_str());
    }
}

void CMainGui::GameNameChanged(CMainGui * Gui)
{
    std::string GameName = g_Settings->LoadStringVal(Game_Name);
    if (GameName.length() > 0)
    {
        std::wstring Caption;
        Caption += stdstr(GameName).ToUTF16();
        Caption += L" | ";
        Caption += Gui->m_WindowTitle;
        SetWindowTextW(Gui->m_hWnd, Caption.c_str());
    }
}
