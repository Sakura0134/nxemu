#include "stdafx.h"
#include <Windows.h>

CMainGui::CMainGui(const wchar_t * WindowTitle) :
    m_hMainWindow(NULL),
    m_ClassName(stdstr_f("NXEmu %s", VER_FILE_VERSION_STR).ToUTF16())
{
    RegisterWinClass();
    Create(WindowTitle);
}

CMainGui::~CMainGui()
{
    if (m_hMainWindow)
    {
        DestroyWindow((HWND)m_hMainWindow);
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
    m_hMainWindow = CreateWindowExW(WS_EX_ACCEPTFILES, m_ClassName.c_str(), WindowTitle, WS_OVERLAPPED | WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS | WS_SYSMENU | WS_MINIMIZEBOX, 5, 5, 640, 480,
        NULL, NULL, GetModuleHandle(NULL), this);
    DWORD blah = GetLastError();
    blah = blah;
}

uint32_t CMainGui::ProcessAllMessages(void)
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
    if (m_hMainWindow)
    {
        ShowWindow((HWND)m_hMainWindow, Visible ? SW_SHOW : SW_HIDE);
    }
}

void * CMainGui::MainGui_Proc(void * hWnd, uint32_t uMsg, uint32_t wParam, uint32_t lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        //record class for future usage
        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
        CMainGui * _this = (CMainGui *)lpcs->lpCreateParams;
        SetProp((HWND)hWnd, "Class", _this);

        _this->m_hMainWindow = hWnd;
    }
    break;
    default:
        return (void *)DefWindowProcW((HWND)hWnd, uMsg, wParam, lParam);
    }
    return (void *)true;
}