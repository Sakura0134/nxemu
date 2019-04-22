#pragma once
#include <nxemu-core\Settings\Debug.h>
#include <nxemu\UserInterface\WTLApp.h>
#include <nxemu\UserInterface\Debugger\DebugDialog.h>
#include <nxemu-core\Machine\Arm64Opcode.h>
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
    std::vector<std::string> m_opAddr;
    std::vector<BRANCHARROW> m_BranchArrows;
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
