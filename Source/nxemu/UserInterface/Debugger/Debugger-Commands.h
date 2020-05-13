#pragma once
#include <nxemu-core\Settings\Debug.h>
#include <nxemu\UserInterface\WTLApp.h>
#include <nxemu\UserInterface\Debugger\DebugDialog.h>
#include <nxemu-core\Machine\Arm64Opcode.h>
#include "Debugger-RegisterTabs.h"
#include <Common\SyncEvent.h>
#include <vector>

class CCommandList :
    public CListImpl<CCommandList>
{
public:
    enum
    {
        COL_ARROWS = 0,
        COL_ADDRESS = 1,
        COL_COMMAND = 2,
        COL_PARAMETERS = 3,
    };
    DECLARE_WND_CLASS(_T("ListCtrl"))

    CCommandList(CDebuggerUI * Debugger);

    void Attach(HWND hWndNew);
    void ShowAddress(uint64_t address, bool top);

    BEGIN_MSG_MAP_EX(CCommandsList)
        CHAIN_MSG_MAP(CListImpl<CCommandList>)
    END_MSG_MAP()

    uint64_t StartAddress(void) const { return m_StartAddress; }
    int GetItemCount();
    const char * GetItemText(int nItem, int nSubItem);
    UINT GetItemMaxEditLen(int nItem, int nSubItem);
    void DrawCustomItem(CDCHandle dcPaint, int nItem, int nSubItem, CRect& rcSubItem);
    BOOL GetItemColours(int nItem, int nSubItem, COLORREF& rgbBackground, COLORREF& rgbText);
    BOOL GetItemSelectedColours(int nItem, int nSubItem, COLORREF& rgbSelectedText);
    void DrawList(CDCHandle dcPaint);
    bool FixRowsVisible(void);

private:
    typedef std::vector<Arm64Opcode> Arm64OpcodeList;

    void AddBranchArrow(int startPos, int endPos);
    void ClearBranchArrows();
    void DrawBranchArrows(CDCHandle &dcPaint);

    typedef struct
    {
        int32_t startPos;
        int32_t endPos;
        int32_t startMargin;
        int32_t endMargin;
        int32_t margin;
    } BRANCHARROW;

    CDebuggerUI * m_Debugger;
    uint64_t m_StartAddress;
    uint32_t m_CommandListRows;
    Arm64OpcodeList m_ops;
    Arm64OpcodeCache m_OpcodeCache;
    std::vector<std::string> m_opAddr;
    std::vector<bool> m_ValidOp;
    std::vector<BRANCHARROW> m_BranchArrows;
};

class CDebugCommandsView :
    public CDebugDialog<CDebugCommandsView>,
    public CDialogResize<CDebugCommandsView>,
    public CDebugSettings
{
public:
    enum { IDD = IDD_Debugger_Commands };

    CDebugCommandsView(CDebuggerUI * debugger, SyncEvent &StepEvent);
    virtual ~CDebugCommandsView(void);

private:
    BEGIN_MSG_MAP_EX(CDebugCommandsView)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_SIZING, OnSizing)
        MESSAGE_HANDLER(WM_VSCROLL, OnScroll)
        COMMAND_HANDLER(IDC_GO_BTN, BN_CLICKED, OnGo)
        COMMAND_HANDLER(IDC_STEP_BTN, BN_CLICKED, OnStep)
        COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnCancel)
        NOTIFY_HANDLER_EX(IDC_REG_TABS, TCN_SELCHANGE, OnRegisterTabChange)
        MSG_WM_DESTROY(OnDestroy)
        CHAIN_MSG_MAP(CDialogResize<CDebugCommandsView>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(CDebugCommandsView)
        DLGRESIZE_CONTROL(IDC_GO_BTN, DLSZ_MOVE_X)
        DLGRESIZE_CONTROL(IDC_STEP_BTN, DLSZ_MOVE_X)
        DLGRESIZE_CONTROL(IDC_REG_TABS, DLSZ_MOVE_X)
        DLGRESIZE_CONTROL(IDC_CMD_LIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
        DLGRESIZE_CONTROL(IDC_SCRL_BAR, DLSZ_MOVE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

    static void StaticWaitingForStepChanged(CDebugCommandsView * _this) { _this->WaitingForStepChanged(); }

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGo(WORD wNotifyCode, WORD wID, HWND hwnd, BOOL& bHandled);
    LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hwnd, BOOL& bHandled);
    LRESULT OnStep(WORD wNotifyCode, WORD wID, HWND hwnd, BOOL& bHandled);
    LRESULT OnRegisterTabChange(NMHDR* pNMHDR);
    LRESULT OnDestroy(void);

    void CPUResume();
    void WaitingForStepChanged(void);

    CCommandList m_CommandList;
    CScrollBar m_Scrollbar;

    CRegisterTabs m_RegisterTabs;
    SyncEvent & m_StepEvent;
    CButton m_StepButton;
    CButton m_GoButton;
};
