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
    DECLARE_WND_CLASS(_T("ListCtrl"))

    CCommandList(CDebuggerUI * Debugger);

    void Attach(HWND hWndNew);

    BEGIN_MSG_MAP_EX(CCommandsList)
        CHAIN_MSG_MAP(CListImpl<CCommandList>)
    END_MSG_MAP()

    UINT GetItemMaxEditLen(int nItem, int nSubItem);
    BOOL GetItemSelectedColours(int nItem, int nSubItem, COLORREF& rgbSelectedText);

private:
    CDebuggerUI * m_Debugger;
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
        MSG_WM_DESTROY(OnDestroy)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(void);

    CCommandList m_CommandList;
};
