#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\Machine\CPU\MemoryManagement.h>
#include <nxemu-core\Machine\Interpreter\InterpreterCPU.h>
#include <nxemu\UserInterface\Debugger\Debugger-Commands.h>
#include <nxemu\UserInterface\Debugger\Debugger.h>
#include <Common\StdString.h>

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
    AddColumn(_T(""), 30, ITEM_IMAGE_NONE, true, ITEM_FORMAT_CUSTOM);
    AddColumn(_T("Address"), 115);
    AddColumn(_T("Command"), 60);
    AddColumn(_T("Parameters"), 148);
    FixRowsVisible();
}

void CCommandList::ShowAddress(uint64_t address, bool top)
{
    bool bOutOfView = address < m_StartAddress || address > m_StartAddress + (m_CommandListRows - 1) * 4;
    if (bOutOfView || top || m_opAddr.size() == 0)
    {
        m_ops.clear();
        m_opAddr.clear();
        m_opName.clear();
        m_opParam.clear();
        m_ValidOp.clear();
        ClearBranchArrows();
        m_StartAddress = address;
        MemoryManagement & ThreadMemory = m_Debugger->DebugThread()->GetSystemThreadPtr()->MMU();

        for (uint32_t i = 0; i < m_CommandListRows; i++)
        {
            uint64_t opAddr = m_StartAddress + (i * 4);
            m_opAddr.push_back(stdstr_f("%016I64X", opAddr).ToUTF16());

            uint32_t insn = 0;
            bool ValidOp = true;
            if (!ThreadMemory.Read32(opAddr, insn))
            {
                ValidOp = false;
            }
            m_ValidOp.push_back(ValidOp);
            if (ValidOp)
            {
                Arm64Opcode OpInfo(m_OpcodeCache, opAddr, insn);
                m_ops.push_back(OpInfo);
                m_opName.push_back(stdstr(OpInfo.Name()).ToUTF16());
                m_opParam.push_back(stdstr(OpInfo.Param()).ToUTF16());

                if (OpInfo.IsBranch())
                {
                    int32_t startPos = i;
                    int64_t endPos = startPos + ((int64_t)(OpInfo.BranchDest() - opAddr) >> 2);
                    if (endPos < -100) { endPos = -100; }
                    if (endPos > 100) { endPos = 100; }

                    AddBranchArrow(startPos, (int32_t)endPos);
                }
            }
            else
            {
                m_opName.push_back(L"********");
                m_opParam.push_back(L"");
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
        uint64_t PC = m_Debugger->DebugThread() != nullptr ? m_Debugger->DebugThread()->GetSystemThreadPtr()->Reg().Get64(Arm64Opcode::ARM64_REG_PC) : 0;
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

const wchar_t * CCommandList::GetItemText(int nItem, int nSubItem)
{
    if (nSubItem == COL_ADDRESS && nItem >= 0 && nItem < m_opAddr.size())
    {
        return m_opAddr[nItem].c_str();
    }
    if (nSubItem == COL_COMMAND && nItem >= 0 && nItem < m_opName.size())
    {
        return m_opName[nItem].c_str();
    }
    if (nSubItem == COL_PARAMETERS && nItem >= 0 && nItem < m_opParam.size())
    {
        return m_opParam[nItem].c_str();
    }
    return L"";
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
        MoveToEx(dcPaint, begX - 1, begY, nullptr);
        LineTo(dcPaint, marginX, begY);
        LineTo(dcPaint, marginX, endY);
        if (bEndVisible)
        {
            LineTo(dcPaint, endX + 2, endY);
        }

        // draw fill line
        CPen hPen(CreatePen(PS_SOLID, 1, color));
        SelectObject(dcPaint, hPen);
        MoveToEx(dcPaint, begX - 1, begY, nullptr);
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

CDebugCommandsView::CDebugCommandsView(CDebuggerUI * debugger, SyncEvent &StepEvent) :
    CDebugDialog<CDebugCommandsView>(debugger),
    m_CommandList(debugger),
    m_RegisterTabs(debugger),
    m_StepEvent(StepEvent)
{
    m_CommandList.RegisterClass();
}

CDebugCommandsView::~CDebugCommandsView()
{
    HideWindow();
}

LRESULT	CDebugCommandsView::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    g_Settings->RegisterChangeCB(Debugger_WaitingForStep, this, (CSettings::SettingChangedFunc)StaticWaitingForStepChanged);

    m_CommandList.Attach(GetDlgItem(IDC_CMD_LIST));
    m_StepButton.Attach(GetDlgItem(IDC_STEP_BTN));
    m_GoButton.Attach(GetDlgItem(IDC_GO_BTN));
    m_RegisterTabs.Attach(GetDlgItem(IDC_REG_TABS));
    m_Scrollbar.Attach(GetDlgItem(IDC_SCRL_BAR));
    DlgResize_Init(false, true);

    m_StepButton.EnableWindow(FALSE);
    m_GoButton.EnableWindow(FALSE);

    // Setup list scrollbar
    m_Scrollbar.SetScrollRange(0, 100, FALSE);
    m_Scrollbar.SetScrollPos(50, TRUE);

    uint64_t PC = m_Debugger->DebugThread() != nullptr ? m_Debugger->DebugThread()->GetSystemThreadPtr()->Reg().Get64(Arm64Opcode::ARM64_REG_PC) : 0;
    m_CommandList.ShowAddress(PC, TRUE);

    if (isStepping())
    {
        m_StepButton.EnableWindow(TRUE);
        m_GoButton.EnableWindow(TRUE);
    }
    WindowCreated();
    return TRUE;
}

LRESULT CDebugCommandsView::OnDestroy(void)
{
    if (g_Settings)
    {
        g_Settings->UnregisterChangeCB(Debugger_WaitingForStep, this, (CSettings::SettingChangedFunc)StaticWaitingForStepChanged);
    }

    m_Scrollbar.Detach();
    m_RegisterTabs.Detach();
    m_GoButton.Detach();
    m_StepButton.Detach();
    m_CommandList.Detach();
    return 0;
}

LRESULT CDebugCommandsView::OnRegisterTabChange(NMHDR* /*pNMHDR*/)
{
    int nPage = m_RegisterTabs.GetCurSel();
    m_RegisterTabs.ShowTab(nPage);
    m_RegisterTabs.RedrawCurrentTab();
    return FALSE;
}

void CDebugCommandsView::CPUResume()
{
    g_Settings->SaveBool(Debugger_SteppingOps, false);
    if (WaitingForStep())
    {
        m_StepEvent.Trigger();
    }
}

LRESULT CDebugCommandsView::OnStep(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hwnd*/, BOOL& /*bHandled*/)
{
    if (WaitingForStep())
    {
        m_StepEvent.Trigger();
    }
    return TRUE;
}

LRESULT CDebugCommandsView::OnGo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hwnd*/, BOOL& /*bHandled*/)
{
    CPUResume();
    return TRUE;
}

LRESULT CDebugCommandsView::OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hwnd*/, BOOL& /*bHandled*/)
{
    EndDialog(0);
    return TRUE;
}
    

LRESULT	CDebugCommandsView::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    if (m_CommandList.FixRowsVisible())
    {
        m_CommandList.ShowAddress(m_CommandList.StartAddress(), true);
    }
    m_RegisterTabs.RedrawCurrentTab();
    return FALSE;
}

LRESULT CDebugCommandsView::OnScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    WORD type = LOWORD(wParam);

    switch (type)
    {
    case SB_LINEUP:
        m_CommandList.ShowAddress(m_CommandList.StartAddress() - 4, TRUE);
        break;
    case SB_LINEDOWN:
        m_CommandList.ShowAddress(m_CommandList.StartAddress() + 4, TRUE);
        break;
    }

    return FALSE;
}

void CDebugCommandsView::WaitingForStepChanged(void)
{
    if (WaitingForStep())
    {
        if (m_Debugger->DebugThread() != nullptr)
        {
            IRegisters & Reg = m_Debugger->DebugThread()->GetSystemThreadPtr()->Reg();
            m_CommandList.ShowAddress(Reg.Get64(Arm64Opcode::ARM64_REG_PC), false);
        }
        m_RegisterTabs.RefreshRegisterTabs();
        m_StepButton.EnableWindow(true);
        m_GoButton.EnableWindow(true);
    }
    else
    {
        m_StepButton.EnableWindow(false);
        m_GoButton.EnableWindow(false);
    }
    m_CommandList.Invalidate();
}
