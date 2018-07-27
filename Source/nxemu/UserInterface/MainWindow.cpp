#include "MainWindow.h"
#include <Common\path.h>
#include <Common\StdString.h>
#include <nxemu-core\Settings\SettingType\SettingsType-Application.h>
#include <nxemu\Settings\UISettings.h>
#include <Windows.h>

CMainGui::CMainGui(const wchar_t * WindowTitle) :
    m_hWnd(NULL),
    m_ClassName(L"NXEmu"),
    m_Menu(this)
{
    RegisterWinClass();
    Create(WindowTitle);
}

CMainGui::~CMainGui()
{
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

void CMainGui::Create(const wchar_t * WindowTitle)
{
    CreateWindowExW(WS_EX_ACCEPTFILES, m_ClassName.c_str(), WindowTitle, WS_OVERLAPPED | WS_CLIPCHILDREN |
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

void CMainGui::AddRecentDir(const char * GameDir)
{
    if (GameDir == NULL) { return; }

    //Get Information about the stored rom list
    size_t MaxRememberedDirs = UISettingsLoadDword(Directory_RecentGameDirCount);
    strlist RecentDirs;
    for (uint32_t i = 0; i < MaxRememberedDirs; i++)
    {
        stdstr RecentDir = UISettingsLoadStringIndex(Directory_RecentGameDirIndex, i);
        if (RecentDir.empty())
        {
            break;
        }
        RecentDirs.push_back(RecentDir);
    }

    //See if the dir is already in the list if so then move it to the top of the list
    for (strlist::iterator iter = RecentDirs.begin(); iter != RecentDirs.end(); iter++)
    {
        if (_stricmp(GameDir, iter->c_str()) != 0)
        {
            continue;
        }
        RecentDirs.erase(iter);
        break;
    }
    RecentDirs.push_front(GameDir);
    if (RecentDirs.size() > MaxRememberedDirs)
    {
        RecentDirs.pop_back();
    }

    uint32_t i = 0;
    for (strlist::iterator iter = RecentDirs.begin(); iter != RecentDirs.end(); iter++)
    {
        UISettingsSaveStringIndex(Directory_RecentGameDirIndex, i++, *iter);
    }
    CSettingTypeApplication::Flush();
}

int CALLBACK SelectDirCallBack(HWND hwnd, DWORD uMsg, DWORD /*lp*/, DWORD lpData)
{
    switch (uMsg)
    {
    case BFFM_INITIALIZED:
        // WParam is TRUE since you are passing a path.
        // It would be FALSE if you were passing a pidl.
        if (lpData)
        {
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        }
        break;
    }
    return 0;
}

LRESULT CMainGui::OnLoadDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    wchar_t Buffer[MAX_PATH], Directory[MAX_PATH];

    stdstr RecentDir = UISettingsLoadStringIndex(Directory_RecentGameDirIndex, 0);
    std::wstring wTitle = wGS(MSG_SELECT_GAME_DIR);

    BROWSEINFOW bi = { 0 };
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = Buffer;
    bi.lpszTitle = wTitle.c_str();
    bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
    bi.lpfn = RecentDir.length() > 0 ? (BFFCALLBACK)SelectDirCallBack : NULL;
    bi.lParam = (LPARAM)(RecentDir.length() > 0 ? RecentDir.c_str() : NULL);

    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl != NULL && SHGetPathFromIDListW(pidl, Directory))
    {
        stdstr path;
        CPath SelectedDir(path.FromUTF16(Directory), "");
        AddRecentDir(SelectedDir);
    }
    return 0;
}

LRESULT CMainGui::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DestroyWindow(m_hWnd);
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
