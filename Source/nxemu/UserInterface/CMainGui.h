#pragma once

class CMainGui
{
public:
    CMainGui(const wchar_t * WindowTitle);
    ~CMainGui(void);

    uint32_t ProcessAllMessages(void);
    void Show(bool ShowWindow); //Show or Hide the current window

private:
    void Create(const wchar_t * WindowTitle);
    bool RegisterWinClass(void);

    static void * __stdcall MainGui_Proc(void *, uint32_t, uint32_t, uint32_t);

    void * m_hMainWindow;
    std::wstring m_ClassName;
};
