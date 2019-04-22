#include <nxemu-core\Machine\CPU\MemoryManagement.h>
#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu\UserInterface\Debugger\Debugger-Commands.h>
#include <nxemu\UserInterface\Debugger\Debugger.h>

CCommandList::CCommandList(CDebuggerUI * Debugger) :
    m_Debugger(Debugger),
    m_StartAddress(0),
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
    FixRowsVisible();
}

void CCommandList::ShowAddress(uint64_t address, bool top)
{
    bool bOutOfView = address < m_StartAddress || address > m_StartAddress + (m_CommandListRows - 1) * 4;
    if (bOutOfView || top || m_opAddr.size() == 0)
    {
        m_opAddr.clear();
        m_StartAddress = address;
        MemoryManagement & ThreadMemory = m_Debugger->Executor()->MMU();

        for (uint32_t i = 0; i < m_CommandListRows; i++)
        {
            uint64_t opAddr = m_StartAddress + i * 4;
            char AddressStr[100]                ;
            sprintf(AddressStr, "%016I64X", opAddr);
            m_opAddr.push_back(AddressStr);

            uint32_t insn = 0;
            bool ValidOp = true;
            if (!ThreadMemory.Read32(opAddr, insn))
            {
                ValidOp = false;
            }
        }
    }
    Invalidate();
}

bool CCommandList::FixRowsVisible(void)
{
    CRect listRect;
    GetWindowRect(listRect);
    uint32_t rowsHeight = listRect.Height() - (m_bShowHeader ? m_nHeaderHeight : 0);
    uint32_t nRows = (rowsHeight / m_nItemHeight);

    if (m_CommandListRows != nRows)
    {
        m_CommandListRows = nRows;
        return true;
    }
    return false;
}

UINT CCommandList::GetItemMaxEditLen(int /*nItem*/, int /*nSubItem*/)
{
    return 0;
}

BOOL CCommandList::GetItemColours(int nItem, int nSubItem, COLORREF& rgbBackground, COLORREF& rgbText)
{
    if (nSubItem > 0)
    {
        uint64_t address = m_StartAddress + (nItem * 4);
        uint64_t PC = m_Debugger->Executor() != NULL ? m_Debugger->Executor()->Reg().Get64(Arm64Opcode::ARM64_REG_PC) : 0;
        bool isPC = address == PC;

        if (isPC)
        {
            rgbBackground = RGB(0x88, 0x88, 0x88);
            rgbText = RGB(0xFF, 0xFF, 0);
        }
        else
        {
            rgbBackground = RGB(0xFF, 0xFF, 0xFF);
            rgbText = RGB(0, 0, 0);
        }
        return true;
    }
    return false;
}

const char * CCommandList::GetItemText(int nItem, int nSubItem)
{
    if (nItem < 0 || nItem >= m_opAddr.size())
    {
        return "";
    }

    switch (nSubItem)
    {
    case COL_ADDRESS: return m_opAddr[nItem].c_str();
    }
    return "";
}

void CCommandList::DrawCustomItem(CDCHandle /*dcPaint*/, int /*nItem*/, int /*nSubItem*/, CRect& /*rcSubItem*/)
{
}

void CCommandList::DrawList(CDCHandle dcPaint)
{
    CListImpl<CCommandList>::DrawList(dcPaint);
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

    uint64_t PC = m_Debugger->Executor() != NULL ? m_Debugger->Executor()->Reg().Get64(Arm64Opcode::ARM64_REG_PC) : 0;
    m_CommandList.ShowAddress(PC, TRUE);

    WindowCreated();
    return TRUE;
}

LRESULT CDebugCommandsView::OnDestroy(void)
{
    m_CommandList.Detach();
    m_CommandList = CCommandList(m_Debugger);
    return 0;
}

LRESULT CDebugCommandsView::OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hwnd*/, BOOL& /*bHandled*/)
{
    EndDialog(0);
    return TRUE;
}
    