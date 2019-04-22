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
        m_ops.clear();
        m_opAddr.clear();
        ClearBranchArrows();
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
            Arm64Opcode OpInfo(opAddr, insn);
            m_ops.push_back(OpInfo);
            if (OpInfo.IsBranch())
            {
                int32_t startPos = i;
                int64_t endPos = startPos + ((int64_t)(OpInfo.BranchDest() - opAddr) >> 2);
                if (endPos < -100) { endPos = -100; }
                if (endPos > 100) { endPos = 100; }

                AddBranchArrow(startPos, (int32_t)endPos);
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
        else if (IsSelected(nItem))
        {
            rgbBackground = m_rgbSelectedItem;
            rgbText = m_rgbSelectedText;
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

    Arm64Opcode &op = m_ops[nItem];
    switch (nSubItem)
    {
    case COL_ADDRESS: return m_opAddr[nItem].c_str();
    case COL_COMMAND: return op.Name();
    case COL_PARAMETERS: return op.Param();
    }
    return "";
}

void CCommandList::DrawCustomItem(CDCHandle /*dcPaint*/, int /*nItem*/, int /*nSubItem*/, CRect& /*rcSubItem*/)
{
}

void CCommandList::DrawList(CDCHandle dcPaint)
{
    CListImpl<CCommandList>::DrawList(dcPaint);
    DrawBranchArrows(dcPaint);
}

void CCommandList::ClearBranchArrows()
{
    m_BranchArrows.clear();
}

void CCommandList::AddBranchArrow(int32_t startPos, int32_t endPos)
{
    int startMargin = 0;
    int endMargin = 0;
    int margin = 0;

    for (size_t i = 0; i < m_BranchArrows.size(); i++)
    {
        BRANCHARROW arrow = m_BranchArrows[i];

        // Arrow's start or end pos within another arrow's stride
        if ((startPos >= arrow.startPos && startPos <= arrow.endPos) ||
            (endPos >= arrow.startPos && endPos <= arrow.endPos) ||
            (arrow.startPos <= startPos && arrow.startPos >= endPos))
        {
            if (margin <= arrow.margin)
            {
                margin = arrow.margin + 1;
            }
        }

        if (startPos == arrow.startPos)
        {
            startMargin = arrow.startMargin + 1;
        }

        if (startPos == arrow.endPos)
        {
            startMargin = arrow.endMargin + 1;
        }

        if (endPos == arrow.startPos)
        {
            endMargin = arrow.startMargin + 1;
        }

        if (endPos == arrow.endPos)
        {
            endMargin = arrow.endMargin + 1;
        }
    }

    m_BranchArrows.push_back({ startPos, endPos, startMargin, endMargin, margin });
}

void CCommandList::DrawBranchArrows(CDCHandle & dcPaint)
{
    static COLORREF colors[] =
    {
        RGB(240, 240, 240), // white
        RGB(30, 135, 255), // blue
        RGB(255, 0, 200), // pink
        RGB(215, 155, 0), // yellow
        RGB(100, 180, 0), // green
        RGB(200, 100, 255), // purple
        RGB(120, 120, 120), // gray
        RGB(0, 220, 160), // cyan
        RGB(255, 100, 0), // orange
        RGB(255, 255, 0), // yellow
    };
    static int32_t nColors = sizeof(colors) / sizeof(COLORREF);
    int32_t colWidth = GetColumnWidth(CCommandList::COL_ARROWS);

    CRect paneRect;
    paneRect.left = -GetScrollPos(SB_HORZ);
    paneRect.right = colWidth;
    paneRect.top = (m_bShowHeader ? m_nHeaderHeight : 0);
    paneRect.bottom = paneRect.top + m_nItemHeight * GetItemCount();

    COLORREF bgColor = RGB(30, 30, 30);
    CBrush hBrushBg(CreateSolidBrush(bgColor));
    FillRect(dcPaint, &paneRect, hBrushBg);

    int32_t baseX = colWidth - 4;
    int32_t baseY = m_nHeaderHeight + 7;

    for (size_t i = 0; i < m_BranchArrows.size(); i++)
    {
        int colorIdx = i % nColors;
        COLORREF color = colors[colorIdx];

        BRANCHARROW arrow = m_BranchArrows[i];

        int32_t begX = baseX - arrow.startMargin * 3;
        int32_t endX = baseX - arrow.endMargin * 3;

        int32_t begY = baseY + arrow.startPos * m_nItemHeight;
        int32_t endY = baseY + arrow.endPos * m_nItemHeight;

        bool bEndVisible = true;

        if (endY < 0)
        {
            endY = 1;
            bEndVisible = false;
        }
        else if (endY > paneRect.bottom)
        {
            endY = paneRect.bottom - 2;
            bEndVisible = false;
        }

        int marginX = baseX - (4 + arrow.margin * 3);

        // draw start pointer
        SetPixel(dcPaint, begX + 0, begY - 1, color);
        SetPixel(dcPaint, begX + 1, begY - 2, color);
        SetPixel(dcPaint, begX + 0, begY + 1, color);
        SetPixel(dcPaint, begX + 1, begY + 2, color);

        // draw outline
        CPen hPenOutline(CreatePen(PS_SOLID, 3, bgColor));
        SelectObject(dcPaint, hPenOutline);
        MoveToEx(dcPaint, begX - 1, begY, NULL);
        LineTo(dcPaint, marginX, begY);
        LineTo(dcPaint, marginX, endY);
        if (bEndVisible)
        {
            LineTo(dcPaint, endX + 2, endY);
        }

        // draw fill line
        CPen hPen(CreatePen(PS_SOLID, 1, color));
        SelectObject(dcPaint, hPen);
        MoveToEx(dcPaint, begX - 1, begY, NULL);
        LineTo(dcPaint, marginX, begY);
        LineTo(dcPaint, marginX, endY);
        if (bEndVisible)
        {
            LineTo(dcPaint, endX + 2, endY);
        }

        // draw end pointer
        if (bEndVisible)
        {
            SetPixel(dcPaint, endX - 0, endY - 1, color);
            SetPixel(dcPaint, endX - 1, endY - 2, color);
            SetPixel(dcPaint, endX - 1, endY - 1, color);
            SetPixel(dcPaint, endX - 0, endY + 1, color);
            SetPixel(dcPaint, endX - 1, endY + 2, color);
            SetPixel(dcPaint, endX - 1, endY + 1, color);
            SetPixel(dcPaint, endX - 1, endY + 3, RGB(30, 30, 30));
            SetPixel(dcPaint, endX - 1, endY - 3, RGB(30, 30, 30));
        }
    }
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
    HideWindow();
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
    