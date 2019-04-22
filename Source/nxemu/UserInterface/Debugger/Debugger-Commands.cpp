#include <nxemu\UserInterface\Debugger\Debugger-Commands.h>

CCommandList::CCommandList(CDebuggerUI * Debugger) :
    m_Debugger(Debugger)
{
}

void CCommandList::Attach(HWND hWndNew)
{
    SubclassWindow(hWndNew);
}
UINT CCommandList::GetItemMaxEditLen(int /*nItem*/, int /*nSubItem*/)
{
    return 0;
}

BOOL CCommandList::GetItemSelectedColours(int nItem, int nSubItem, COLORREF& rgbSelectedText)
{
    COLORREF rgbBackground;
    return GetItemColours(nItem, nSubItem, rgbBackground, rgbSelectedText);
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
    WindowCreated();
    return TRUE;
}

LRESULT CDebugCommandsView::OnDestroy(void)
{
    return 0;
}
