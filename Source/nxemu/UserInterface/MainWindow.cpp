#include "MainWindow.h"
#include <Common\StdString.h>
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

LRESULT CMainGui::OnLoadDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    wchar_t Buffer[MAX_PATH], Directory[MAX_PATH];
    BROWSEINFOW bi;

    std::wstring wTitle = wGS(MSG_SELECT_GAME_DIR);
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = Buffer;
    bi.lpszTitle = wTitle.c_str();
    bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
    bi.lpfn = NULL;
    bi.lParam = NULL;

    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl != NULL && SHGetPathFromIDListW(pidl, Directory))
    {
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
