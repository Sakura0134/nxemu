#pragma once
#include <nxemu-core\Settings\Debug.h>
#include <nxemu\UserInterface\WTLApp.h>
#include <nxemu\UserInterface\Debugger\DebugDialog.h>
#include <Common\SyncEvent.h>
#include <vector>

class CCommandList :
    public CListImpl<CCommandList>
{
public:
    enum
    {
        COL_ADDRESS = 1,
    };
    DECLARE_WND_CLASS(_T("ListCtrl"))

    CCommandList(CDebuggerUI * Debugger);

    void Attach(HWND hWndNew);
    void ShowAddress(uint64_t address, bool top);

    BEGIN_MSG_MAP_EX(CCommandsList)
        CHAIN_MSG_MAP(CListImpl<CCommandList>)
    END_MSG_MAP()

    int GetItemCount();
    const char * GetItemText(int nItem, int nSubItem);
    UINT GetItemMaxEditLen(int nItem, int nSubItem);
    void DrawCustomItem(CDCHandle dcPaint, int nItem, int nSubItem, CRect& rcSubItem);
    BOOL GetItemColours(int nItem, int nSubItem, COLORREF& rgbBackground, COLORREF& rgbText);
    BOOL GetItemSelectedColours(int nItem, int nSubItem, COLORREF& rgbSelectedText);
    void DrawList(CDCHandle dcPaint);
    bool FixRowsVisible(void);

private:
    CDebuggerUI * m_Debugger;
    uint64_t m_StartAddress;
    uint32_t m_CommandListRows;
    std::vector<std::string> m_opAddr;
};

class CDebugCommandsView :
    public CDebugDialog<CDebugCommandsView>,
    public CDebugSettings
{
public:
    enum { IDD = IDD_Debugger_Commands };

    CDebugCommandsView(CDebuggerUI * debugger);
    virtual ~CDebugCommandsView(void);

private:
    BEGIN_MSG_MAP_EX(CDebugCommandsView)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnCancel)
        MSG_WM_DESTROY(OnDestroy)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hwnd, BOOL& bHandled);
    LRESULT OnDestroy(void);

    CCommandList m_CommandList;
};
