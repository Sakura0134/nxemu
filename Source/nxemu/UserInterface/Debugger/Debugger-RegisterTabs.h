#pragma once
#include <nxemu\UserInterface\WTLApp.h>
#include <nxemu\UserInterface\Debugger\Debugger.h>
#include <vector>

template <class T>
class CRegisterTabImpl :
    public CDialogImpl<T>
{
protected:
    bool CreateTab(HWND hParent, const RECT & rcDispay);
    void InitRegisterEdits64(CEdit* edits, const WORD* ctrlIds, uint32_t ctrlIdsCount);
};

class CGPRTab64 :
    public CRegisterTabImpl<CGPRTab64>
{
    BEGIN_MSG_MAP_EX(CGPRTab64)
    END_MSG_MAP()

    enum { IDD = IDD_Debugger_RegGPR64 };

public:
    CGPRTab64(HWND hParent, const RECT & rcDispay);
    ~CGPRTab64();

    void RefreshValues(CPUExecutor * Executor);

private:
    CGPRTab64(void);                        // Disable default constructor
    CGPRTab64(const CGPRTab64&);            // Disable copy constructor
    CGPRTab64& operator=(const CGPRTab64&); // Disable assignment

    static constexpr WORD RegisterIds[] =
    {
        IDC_X0_EDIT,  IDC_X1_EDIT,  IDC_X2_EDIT,  IDC_X3_EDIT,
        IDC_X4_EDIT,  IDC_X5_EDIT,  IDC_X6_EDIT,  IDC_X7_EDIT,
        IDC_X8_EDIT,  IDC_X9_EDIT,  IDC_X10_EDIT, IDC_X11_EDIT,
        IDC_X12_EDIT, IDC_X13_EDIT, IDC_X14_EDIT, IDC_X15_EDIT,
        IDC_X16_EDIT, IDC_X17_EDIT, IDC_X18_EDIT, IDC_X19_EDIT,
        IDC_X20_EDIT, IDC_X21_EDIT, IDC_X22_EDIT, IDC_X23_EDIT,
        IDC_X24_EDIT, IDC_X25_EDIT, IDC_X26_EDIT, IDC_X27_EDIT,
        IDC_X28_EDIT, IDC_X29_EDIT, IDC_X30_EDIT, IDC_SP_EDIT,
    };

    CEdit m_Register[sizeof(RegisterIds) / sizeof(RegisterIds[0])];
};

class CGPRTab32 :
    public CRegisterTabImpl<CGPRTab32>
{
    BEGIN_MSG_MAP_EX(CGPRTab32)
    END_MSG_MAP()

    enum { IDD = IDD_Debugger_RegGPR32 };

public:
    CGPRTab32(HWND hParent, const RECT & rcDispay);
    ~CGPRTab32();

    void RefreshValues(CPUExecutor * Executor);

private:
    CGPRTab32(void);                       // Disable default constructor
    CGPRTab32(const CGPRTab32&);             // Disable copy constructor
    CGPRTab32& operator=(const CGPRTab32&);  // Disable assignment

    static constexpr WORD RegisterIds[] =
    {
        IDC_W0_EDIT,  IDC_W1_EDIT,  IDC_W2_EDIT,  IDC_W3_EDIT,
        IDC_W4_EDIT,  IDC_W5_EDIT,  IDC_W6_EDIT,  IDC_W7_EDIT,
        IDC_W8_EDIT,  IDC_W9_EDIT,  IDC_W10_EDIT, IDC_W11_EDIT,
        IDC_W12_EDIT, IDC_W13_EDIT, IDC_W14_EDIT, IDC_W15_EDIT,
        IDC_W16_EDIT, IDC_W17_EDIT, IDC_W18_EDIT, IDC_W19_EDIT,
        IDC_W20_EDIT, IDC_W21_EDIT, IDC_W22_EDIT, IDC_W23_EDIT,
        IDC_W24_EDIT, IDC_W25_EDIT, IDC_W26_EDIT, IDC_W27_EDIT,
        IDC_W28_EDIT, IDC_W29_EDIT, IDC_W30_EDIT,
    };

    CEdit m_Register[sizeof(RegisterIds) / sizeof(RegisterIds[0])];
};

class CRegQTab :
    public CRegisterTabImpl<CRegQTab>
{
    BEGIN_MSG_MAP_EX(CRegQTab)
    END_MSG_MAP()

    enum { IDD = IDD_Debugger_RegVFP };

public:
    CRegQTab(HWND hParent, const RECT & rcDispay);
    ~CRegQTab();

    void RefreshValues(CPUExecutor * Executor);

private:
    CRegQTab(void);                       // Disable default constructor
    CRegQTab(const CRegQTab&);            // Disable copy constructor
    CRegQTab& operator=(const CRegQTab&); // Disable assignment

    static constexpr WORD RegisterIds[] =
    {
        IDC_Q0_EDIT,  IDC_Q1_EDIT,  IDC_Q2_EDIT,  IDC_Q3_EDIT,
        IDC_Q4_EDIT,  IDC_Q5_EDIT,  IDC_Q6_EDIT,  IDC_Q7_EDIT,
        IDC_Q8_EDIT,  IDC_Q9_EDIT,  IDC_Q10_EDIT, IDC_Q11_EDIT,
        IDC_Q12_EDIT, IDC_Q13_EDIT, IDC_Q14_EDIT, IDC_Q15_EDIT,
    };

    CEdit m_Register[sizeof(RegisterIds) / sizeof(RegisterIds[0])];
};

class CRegQTab2 :
    public CRegisterTabImpl<CRegQTab2>
{
    BEGIN_MSG_MAP_EX(CRegQTab2)
    END_MSG_MAP()

    enum { IDD = IDD_Debugger_RegVFP2 };

public:
    CRegQTab2(HWND hParent, const RECT & rcDispay);
    ~CRegQTab2();

    void RefreshValues(CPUExecutor * Executor);

private:
    CRegQTab2(void);                       // Disable default constructor
    CRegQTab2(const CRegQTab2&);            // Disable copy constructor
    CRegQTab2& operator=(const CRegQTab2&); // Disable assignment

    static constexpr WORD RegisterIds[] =
    {
        IDC_Q16_EDIT, IDC_Q17_EDIT, IDC_Q18_EDIT, IDC_Q19_EDIT,
        IDC_Q20_EDIT, IDC_Q21_EDIT, IDC_Q22_EDIT, IDC_Q23_EDIT,
        IDC_Q24_EDIT, IDC_Q25_EDIT, IDC_Q26_EDIT, IDC_Q27_EDIT,
        IDC_Q28_EDIT, IDC_Q29_EDIT, IDC_Q30_EDIT, IDC_Q31_EDIT,
    };

    CEdit m_Register[sizeof(RegisterIds) / sizeof(RegisterIds[0])];
};

class CPStateTab :
    public CRegisterTabImpl<CPStateTab>
{
    BEGIN_MSG_MAP_EX(CPStateTab)
    END_MSG_MAP()

    enum { IDD = IDD_Debugger_RegPSTATE };

public:
    CPStateTab(HWND hParent, const RECT & rcDispay);
    ~CPStateTab();

    void RefreshValues(CPUExecutor * Executor);

private:
    CPStateTab(void);                       // Disable default constructor
    CPStateTab(const CPStateTab&);             // Disable copy constructor
    CPStateTab& operator=(const CPStateTab&);  // Disable assignment

    static constexpr WORD RegisterIds[] =
    {
        IDC_PSTATE_EDIT,   IDC_PSTATE_N_EDIT,  IDC_PSTATE_Z_EDIT,  IDC_PSTATE_C_EDIT,
        IDC_PSTATE_V_EDIT, IDC_PSTATE_EL_EDIT,  
    };

    CEdit m_Register[sizeof(RegisterIds) / sizeof(RegisterIds[0])];
};

class CRegisterTabs :
    public CTabCtrl
{
public:
    CRegisterTabs(CDebuggerUI * debugger);
    ~CRegisterTabs();

    void Attach(HWND hWndNew);
    void Detach();

    void ShowTab(int nPage);
    void RedrawCurrentTab();
    void RefreshEdits();

private:
    CRegisterTabs(void);             // Disable default constructor
    CRegisterTabs(const CRegisterTabs&);             // Disable copy constructor
    CRegisterTabs& operator=(const CRegisterTabs&);  // Disable assignment

    void AddTab(char* caption, CWindow * tabWin);
    CRect GetPageRect();

    CDebuggerUI * m_Debugger;
    std::vector<CWindow*> m_TabWindows;
    CGPRTab64 * m_GPRTab64;
    CGPRTab32 * m_GPRTab32;
    CRegQTab * m_RegQTab;
    CRegQTab2 * m_RegQTab2;
    CPStateTab * m_PStateTab;
};