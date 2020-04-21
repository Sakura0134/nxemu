#include "Debugger-RegisterTabs.h"
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\Registers.h>
#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <Common\StdString.h>
#include "Debugger-RegisterTabs.h"
#include <nxemu-core\Machine\CPU\CPUExecutor.h>

template <class T>
bool CRegisterTabImpl<T>::CreateTab(HWND hParent, const RECT & rcDispay)
{
    BOOL result = m_thunk.Init(NULL, NULL);
    if (result == FALSE)
    {
        SetLastError(ERROR_OUTOFMEMORY);
        return false;
    }

    _AtlWinModule.AddCreateWndData(&m_thunk.cd, this);
#ifdef _DEBUG
    m_bModal = false;
#endif //_DEBUG
    m_hWnd = ::CreateDialogParamW(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCEW(static_cast<T*>(this)->IDD), hParent, T::StartDialogProc, NULL);
    if (m_hWnd == NULL)
    {
        return false;
    }
    SetWindowPos(HWND_TOP, &rcDispay, SWP_HIDEWINDOW);
    return true;
}

template <class T>
void CRegisterTabImpl<T>::InitRegisterEdits64(CEdit* edits, const WORD* ctrlIds, uint32_t ctrlIdsCount)
{
    for (int i = 0, n = ctrlIdsCount; i < n; i++)
    {
        edits[i].Attach(GetDlgItem(ctrlIds[i]));
    }
}

CGPRTab64::CGPRTab64(HWND hParent, const RECT & rcDispay)
{
    CreateTab(hParent, rcDispay);
    InitRegisterEdits64(m_Register, RegisterIds, sizeof(RegisterIds) / sizeof(RegisterIds[0]));
}

CGPRTab64::~CGPRTab64()
{
    DestroyWindow();
}

void CGPRTab64::RefreshValues(CPUExecutor * Executor)
{
    static constexpr Arm64Opcode::arm64_reg Register[sizeof(m_Register) / sizeof(m_Register[0])] =
    {
        Arm64Opcode::ARM64_REG_X0,  Arm64Opcode::ARM64_REG_X1,  Arm64Opcode::ARM64_REG_X2,  Arm64Opcode::ARM64_REG_X3,
        Arm64Opcode::ARM64_REG_X4,  Arm64Opcode::ARM64_REG_X5,  Arm64Opcode::ARM64_REG_X6,  Arm64Opcode::ARM64_REG_X7,
        Arm64Opcode::ARM64_REG_X8,  Arm64Opcode::ARM64_REG_X9,  Arm64Opcode::ARM64_REG_X10, Arm64Opcode::ARM64_REG_X11,
        Arm64Opcode::ARM64_REG_X12, Arm64Opcode::ARM64_REG_X13, Arm64Opcode::ARM64_REG_X14, Arm64Opcode::ARM64_REG_X15,
        Arm64Opcode::ARM64_REG_X16, Arm64Opcode::ARM64_REG_X17, Arm64Opcode::ARM64_REG_X18, Arm64Opcode::ARM64_REG_X19,
        Arm64Opcode::ARM64_REG_X20, Arm64Opcode::ARM64_REG_X21, Arm64Opcode::ARM64_REG_X22, Arm64Opcode::ARM64_REG_X23,
        Arm64Opcode::ARM64_REG_X24, Arm64Opcode::ARM64_REG_X25, Arm64Opcode::ARM64_REG_X26, Arm64Opcode::ARM64_REG_X27,
        Arm64Opcode::ARM64_REG_X28, Arm64Opcode::ARM64_REG_X29, Arm64Opcode::ARM64_REG_X30, Arm64Opcode::ARM64_REG_SP,
    };

    CRegisters * Reg = Executor ? &Executor->Reg() : NULL;
    if (Reg)
    {
        for (size_t i = 0, n = sizeof(m_Register) / sizeof(m_Register[0]); i < n; i++)
        {
            m_Register[i].SetWindowText(stdstr_f("0x%016I64X", Reg->Get64(Register[i])).c_str());
        }
    }
}

CGPRTab32::CGPRTab32(HWND hParent, const RECT & rcDispay)
{
    CreateTab(hParent, rcDispay);
    for (int i = 0, n = sizeof(RegisterIds) / sizeof(RegisterIds[0]); i < n; i++)
    {
        m_Register[i].Attach(GetDlgItem(RegisterIds[i]));
    }
}

CGPRTab32::~CGPRTab32()
{
    DestroyWindow();
}

void CGPRTab32::RefreshValues(CPUExecutor * Executor)
{
    static constexpr Arm64Opcode::arm64_reg Register[sizeof(m_Register) / sizeof(m_Register[0])] =
    {
        Arm64Opcode::ARM64_REG_W0,  Arm64Opcode::ARM64_REG_W1,  Arm64Opcode::ARM64_REG_W2,  Arm64Opcode::ARM64_REG_W3,
        Arm64Opcode::ARM64_REG_W4,  Arm64Opcode::ARM64_REG_W5,  Arm64Opcode::ARM64_REG_W6,  Arm64Opcode::ARM64_REG_W7,
        Arm64Opcode::ARM64_REG_W8,  Arm64Opcode::ARM64_REG_W9,  Arm64Opcode::ARM64_REG_W10, Arm64Opcode::ARM64_REG_W11,
        Arm64Opcode::ARM64_REG_W12, Arm64Opcode::ARM64_REG_W13, Arm64Opcode::ARM64_REG_W14, Arm64Opcode::ARM64_REG_W15,
        Arm64Opcode::ARM64_REG_W16, Arm64Opcode::ARM64_REG_W17, Arm64Opcode::ARM64_REG_W18, Arm64Opcode::ARM64_REG_W19,
        Arm64Opcode::ARM64_REG_W20, Arm64Opcode::ARM64_REG_W21, Arm64Opcode::ARM64_REG_W22, Arm64Opcode::ARM64_REG_W23,
        Arm64Opcode::ARM64_REG_W24, Arm64Opcode::ARM64_REG_W25, Arm64Opcode::ARM64_REG_W26, Arm64Opcode::ARM64_REG_W27,
        Arm64Opcode::ARM64_REG_W28, Arm64Opcode::ARM64_REG_W29, Arm64Opcode::ARM64_REG_W30,
    };

    CRegisters * Reg = Executor ? &Executor->Reg() : NULL;
    if (Reg)
    {
        for (size_t i = 0, n = sizeof(m_Register) / sizeof(m_Register[0]); i < n; i++)
        {
            m_Register[i].SetWindowText(stdstr_f("0x%08X", Reg->Get32(Register[i])).c_str());
        }
    }
}

CRegQTab::CRegQTab(HWND hParent, const RECT & rcDispay)
{
    CreateTab(hParent, rcDispay);
    for (int i = 0, n = sizeof(RegisterIds) / sizeof(RegisterIds[0]); i < n; i++)
    {
        m_Register[i].Attach(GetDlgItem(RegisterIds[i]));
    }
}

CRegQTab::~CRegQTab()
{
    DestroyWindow();
}

void CRegQTab::RefreshValues(CPUExecutor * Executor)
{
    static constexpr Arm64Opcode::arm64_reg Register[sizeof(m_Register) / sizeof(m_Register[0])] =
    {
        Arm64Opcode::ARM64_REG_Q0,  Arm64Opcode::ARM64_REG_Q1,  Arm64Opcode::ARM64_REG_Q2,  Arm64Opcode::ARM64_REG_Q3,
        Arm64Opcode::ARM64_REG_Q4,  Arm64Opcode::ARM64_REG_Q5,  Arm64Opcode::ARM64_REG_Q6,  Arm64Opcode::ARM64_REG_Q7,
        Arm64Opcode::ARM64_REG_Q8,  Arm64Opcode::ARM64_REG_Q9,  Arm64Opcode::ARM64_REG_Q10, Arm64Opcode::ARM64_REG_Q11,
        Arm64Opcode::ARM64_REG_Q12, Arm64Opcode::ARM64_REG_Q13, Arm64Opcode::ARM64_REG_Q14, Arm64Opcode::ARM64_REG_Q15,
    };

    CRegisters * Reg = Executor ? &Executor->Reg() : NULL;
    if (Reg)
    {
        for (size_t i = 0, n = sizeof(m_Register) / sizeof(m_Register[0]); i < n; i++)
        {
            uint64_t hiValue, loValue;
            Reg->Get128(Register[i], hiValue, loValue);
            m_Register[i].SetWindowText(stdstr_f("0x%08X-%08X-%08X-%08X", (uint32_t)(hiValue >> 0x32), (uint32_t)(hiValue & 0xFFFFFFFF), (uint32_t)(loValue >> 0x32), (uint32_t)(loValue & 0xFFFFFFFF)).c_str());
        }
    }
}

CRegQTab2::CRegQTab2(HWND hParent, const RECT & rcDispay)
{
    CreateTab(hParent, rcDispay);
    for (int i = 0, n = sizeof(RegisterIds) / sizeof(RegisterIds[0]); i < n; i++)
    {
        m_Register[i].Attach(GetDlgItem(RegisterIds[i]));
    }
}

CRegQTab2::~CRegQTab2()
{
    DestroyWindow();
}

void CRegQTab2::RefreshValues(CPUExecutor * Executor)
{
    static constexpr Arm64Opcode::arm64_reg Register[sizeof(m_Register) / sizeof(m_Register[0])] =
    {
        Arm64Opcode::ARM64_REG_Q16, Arm64Opcode::ARM64_REG_Q17, Arm64Opcode::ARM64_REG_Q18, Arm64Opcode::ARM64_REG_Q19,
        Arm64Opcode::ARM64_REG_Q20, Arm64Opcode::ARM64_REG_Q21, Arm64Opcode::ARM64_REG_Q22, Arm64Opcode::ARM64_REG_Q23,
        Arm64Opcode::ARM64_REG_Q24, Arm64Opcode::ARM64_REG_Q25, Arm64Opcode::ARM64_REG_Q26, Arm64Opcode::ARM64_REG_Q27,
        Arm64Opcode::ARM64_REG_Q28, Arm64Opcode::ARM64_REG_Q29, Arm64Opcode::ARM64_REG_Q30, Arm64Opcode::ARM64_REG_Q31,
    };

    CRegisters * Reg = Executor ? &Executor->Reg() : NULL;
    if (Reg)
    {
        for (size_t i = 0, n = sizeof(m_Register) / sizeof(m_Register[0]); i < n; i++)
        {
            uint64_t hiValue, loValue;
            Reg->Get128(Register[i], hiValue, loValue);
            m_Register[i].SetWindowText(stdstr_f("0x%08X-%08X-%08X-%08X", (uint32_t)(hiValue >> 0x32), (uint32_t)(hiValue & 0xFFFFFFFF), (uint32_t)(loValue >> 0x32), (uint32_t)(loValue & 0xFFFFFFFF)).c_str());
        }
    }
}

CPStateTab::CPStateTab(HWND hParent, const RECT & rcDispay)
{
    CreateTab(hParent, rcDispay);
    for (int i = 0, n = sizeof(RegisterIds) / sizeof(RegisterIds[0]); i < n; i++)
    {
        m_Register[i].Attach(GetDlgItem(RegisterIds[i]));
    }
}

CPStateTab::~CPStateTab()
{
    DestroyWindow();
}

void CPStateTab::RefreshValues(CPUExecutor * Executor)
{
    CRegisters * Reg = Executor ? &Executor->Reg() : NULL;
    if (Reg)
    {
        const CRegisters::PSTATE & pstate = Reg->GetPstate();
        m_Register[0].SetWindowText(stdstr_f("0x%08X", pstate.value).c_str());
        m_Register[1].SetWindowText(stdstr_f("%d", pstate.N).c_str());
        m_Register[2].SetWindowText(stdstr_f("%d", pstate.Z).c_str());
        m_Register[3].SetWindowText(stdstr_f("%d", pstate.C).c_str());
        m_Register[4].SetWindowText(stdstr_f("%d", pstate.V).c_str());
        m_Register[5].SetWindowText(stdstr_f("%d", pstate.EL).c_str());
    }
}

CRegisterTabs::CRegisterTabs(CDebuggerUI * Debugger) :
    m_Debugger(Debugger),
    m_GPRTab64(NULL),
    m_GPRTab32(NULL),
    m_PStateTab(NULL)
{
}

CRegisterTabs::~CRegisterTabs(void)
{
}

void CRegisterTabs::Attach(HWND hWndNew)
{
    CTabCtrl::Attach(hWndNew);

    CWindow parentWin = GetParent();
    CRect pageRect = GetPageRect();
    m_GPRTab64 = new CGPRTab64(parentWin, pageRect);
    m_GPRTab32 = new CGPRTab32(parentWin, pageRect);
    m_RegQTab = new CRegQTab(parentWin, pageRect);
    m_RegQTab2 = new CRegQTab2(parentWin, pageRect);
    m_PStateTab = new CPStateTab(parentWin, pageRect);

    AddTab("GPR-64 (X)", m_GPRTab64);
    AddTab("GPR-32 (W)", m_GPRTab32);
    AddTab("VFP (Q0-Q15)", m_RegQTab);
    AddTab("VFP (Q16-Q31)", m_RegQTab2);
    AddTab("PState", m_PStateTab);
    RefreshEdits();
    RedrawCurrentTab();
}

void CRegisterTabs::Detach(void)
{
    delete m_GPRTab64;
    m_GPRTab64 = NULL;
    delete m_GPRTab32;
    m_GPRTab32 = NULL;
    delete m_RegQTab;
    m_RegQTab = NULL;
    delete m_RegQTab2;
    m_RegQTab2 = NULL;
    delete m_PStateTab;
    m_PStateTab = NULL;
    m_TabWindows.clear();

    CTabCtrl::Detach();
}

void CRegisterTabs::RefreshEdits()
{
    CPUExecutor * Executor = m_Debugger->Executor();
    m_GPRTab64->RefreshValues(Executor);
    m_GPRTab32->RefreshValues(Executor);
    m_RegQTab->RefreshValues(Executor);
    m_RegQTab2->RefreshValues(Executor);
    m_PStateTab->RefreshValues(Executor);
}

CRect CRegisterTabs::GetPageRect()
{
    CRect pageRect;
    GetWindowRect(&pageRect);
    GetParent().ScreenToClient(&pageRect);
    AdjustRect(FALSE, &pageRect);
    return pageRect;
}

void CRegisterTabs::AddTab(char* caption, CWindow * tabWin)
{
    if (tabWin == NULL)
    {
        return;
    }
    AddItem(caption);

    m_TabWindows.push_back(tabWin);
    if (m_TabWindows.size() == 1)
    {
        ShowTab(0);
    }
}

void CRegisterTabs::ShowTab(int nPage)
{
    for (int i = 0; i < m_TabWindows.size(); i++)
    {
        if (nPage == i)
        {
            continue;
        }
        m_TabWindows[i]->ShowWindow(SW_HIDE);
    }

    CRect pageRect = GetPageRect();
    m_TabWindows[nPage]->SetWindowPos(HWND_TOP, pageRect.left, pageRect.top, pageRect.Width(), pageRect.Height(), SWP_SHOWWINDOW);
}

void CRegisterTabs::RedrawCurrentTab()
{
    ShowTab(GetCurSel());
}
