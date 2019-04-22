#include <nxemu\UserInterface\Debugger\Debugger-Commands.h>

CCommandList::CCommandList(CDebuggerUI * Debugger) :
    m_Debugger(Debugger),
    m_CommandListRows(1)
{
}

void CCommandList::Attach(HWND hWndNew)
{
    SubclassWindow(hWndNew);
    ShowHeaderSort(false);
    AddColumn("", 30, ITEM_IMAGE_NONE, true, ITEM_FORMAT_CUSTOM);
    AddColumn("Address", 115);
    AddColumn("Command", 60);
    AddColumn("Parameters", 148);
}
UINT CCommandList::GetItemMaxEditLen(int /*nItem*/, int /*nSubItem*/)
{
    return 0;
}

const char * CCommandList::GetItemText(int nItem, int nSubItem)
{
    return "";
}

void CCommandList::DrawCustomItem(CDCHandle /*dcPaint*/, int /*nItem*/, int /*nSubItem*/, CRect& /*rcSubItem*/)
{
}

BOOL CCommandList::GetItemSelectedColours(int nItem, int nSubItem, COLORREF& rgbSelectedText)
{
    COLORREF rgbBackground;
    return GetItemColours(nItem, nSubItem, rgbBackground, rgbSelectedText);
}

int CCommandList::GetItemCount()
{
    return m_CommandListRows;
}

CDebugCommandsView::CDebugCommandsView(CDebuggerUI * debugger) :
    CDebugDialog<CDebugCommandsView>(debugger),
    m_CommandList(debugger)
{
    m_CommandList.RegisterClass();
}

CDebugCommandsView::~CDebugCommandsView()
{
}

LRESULT	CDebugCommandsView::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    m_CommandList.Attach(GetDlgItem(IDC_CMD_LIST));
    WindowCreated();
    return TRUE;
}

LRESULT CDebugCommandsView::OnDestroy(void)
{
    m_CommandList.Detach();
    return 0;
}

LRESULT CDebugCommandsView::OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hwnd*/, BOOL& /*bHandled*/)
{
    EndDialog(0);
    return TRUE;
}
