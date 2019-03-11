#include "MainWindow.h"
#include <Common\path.h>
#include <Common\StdString.h>
#include <nxemu-core\Machine\SwitchRom.h>
#include <nxemu-core\Settings\SettingType\SettingsType-Application.h>
#include <nxemu\Settings\UISettings.h>
#include <nxemu\UserInterface\SwitchKeysConfig.h>
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

LRESULT CMainGui::OnOpenRom(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    std::string File = ChooseFileToOpen(m_hWnd);
    if (File.length() == 0)
    {
        return 0;
    }
    LaunchSwitchRom(File.c_str());
    return 0;
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

LRESULT CMainGui::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DestroyWindow(m_hWnd);
    return 0;
}

LRESULT CMainGui::OnDebugLogging(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EditLoggingOptions(m_hWnd);
    return 0;
}

LRESULT CMainGui::OnSwitchKeys(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CKeysConfig KeysConfig;
	KeysConfig.Display(m_hWnd);
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
