#include <nxemu\UserInterface\Debugger\Debugger.h>
#include <nxemu\UserInterface\WTLApp.h>
#include <nxemu-core\Settings\SettingsID.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\Settings\SettingType\SettingsType-Application.h>
#include <nxemu-core\Trace.h>
#include <nxemu-core\SystemGlobals.h>
#include <string>
#include "resource.h"

static void SetComboBoxIndex(CComboBox & cmb, uint32_t data)
{
    cmb.SetCurSel(0);
    for (DWORD i = 0, n = cmb.GetCount(); i < n; i++)
    {
        if (cmb.GetItemData(i) == data)
        {
            cmb.SetCurSel(i);
            break;
        }
    }
}

class CLogSettings;

class COptionsSheet : public CPropertySheetImpl < COptionsSheet >
{
public:
    // Construction
    COptionsSheet(const wchar_t * title, UINT /*uStartPage*/ = 0, HWND /*hWndParent*/ = NULL);
    ~COptionsSheet();

    // Maps
    BEGIN_MSG_MAP(COptionsSheet)
        CHAIN_MSG_MAP(CPropertySheetImpl<COptionsSheet>)
    END_MSG_MAP()

private:
    // Property pages
    CLogSettings * m_pgLogSettings;
    std::wstring m_Title;
};

class CLogSettings :
    public CPropertyPageImpl<CLogSettings>
{
public:
    enum { IDD = IDD_Debugger_logging };

    BEGIN_MSG_MAP(CLogSettings)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_HANDLER_EX(IDC_CMB_TRACE_APPINIT, CBN_SELCHANGE, ItemChanged)
        COMMAND_HANDLER_EX(IDC_CMB_TRACE_APPCLEANUP, CBN_SELCHANGE, ItemChanged)
        COMMAND_HANDLER_EX(IDC_CMB_TRACE_SERVICECALL, CBN_SELCHANGE, ItemChanged)
        COMMAND_HANDLER_EX(IDC_CMB_TRACE_MEMORY, CBN_SELCHANGE, ItemChanged)
        COMMAND_HANDLER_EX(IDC_CMB_TRACE_GAMEFILE, CBN_SELCHANGE, ItemChanged)
        CHAIN_MSG_MAP(CPropertyPageImpl<CLogSettings>)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        m_cmbTraceAppInit.Attach(GetDlgItem(IDC_CMB_TRACE_APPINIT));
        m_cmbTraceAppCleanup.Attach(GetDlgItem(IDC_CMB_TRACE_APPCLEANUP));
        m_cmbTraceServiceCall.Attach(GetDlgItem(IDC_CMB_TRACE_SERVICECALL));
        m_cmbTraceHleKernel.Attach(GetDlgItem(IDC_CMB_TRACE_HLEKERNEL));
        m_cmbTraceMemory.Attach(GetDlgItem(IDC_CMB_TRACE_MEMORY));
        m_cmbTraceGameFile.Attach(GetDlgItem(IDC_CMB_TRACE_GAMEFILE));

        struct 
        {
            CComboBox & cmb;
            SettingID SettingId;
        } TraceCMB[] =
        {
            { m_cmbTraceAppInit, Debugger_TraceAppInit },
            { m_cmbTraceAppCleanup, Debugger_TraceAppCleanup },
            { m_cmbTraceServiceCall, Debugger_TraceServiceCall },
            { m_cmbTraceHleKernel, Debugger_TraceHleKernel },
            { m_cmbTraceMemory, Debugger_TraceMemory },
            { m_cmbTraceGameFile, Debugger_TraceGameFile },
        };

        for (size_t i = 0, n = sizeof(TraceCMB) / sizeof(TraceCMB[0]); i < n; i++)
        {
            TraceCMB[i].cmb.SetItemData(TraceCMB[i].cmb.AddString(L"Error"), TraceError);
            TraceCMB[i].cmb.SetItemData(TraceCMB[i].cmb.AddString(L"Warning"), TraceWarning);
            TraceCMB[i].cmb.SetItemData(TraceCMB[i].cmb.AddString(L"Notice"), TraceNotice);
            TraceCMB[i].cmb.SetItemData(TraceCMB[i].cmb.AddString(L"Info"), TraceInfo);
            TraceCMB[i].cmb.SetItemData(TraceCMB[i].cmb.AddString(L"Debug"), TraceDebug);
            TraceCMB[i].cmb.SetItemData(TraceCMB[i].cmb.AddString(L"Verbose"), TraceVerbose);
            SetComboBoxIndex(TraceCMB[i].cmb, (uint32_t)g_Settings->LoadDword(TraceCMB[i].SettingId));
        }
        return TRUE;
    }

    bool OnApply()
    {
        struct {
            CComboBox & cmb;
            SettingID SettingId;
        } TraceCMB[] =
        {
            { m_cmbTraceAppInit, Debugger_TraceAppInit },
            { m_cmbTraceAppCleanup, Debugger_TraceAppCleanup },
            { m_cmbTraceServiceCall, Debugger_TraceServiceCall },
            { m_cmbTraceHleKernel, Debugger_TraceHleKernel },
            { m_cmbTraceMemory, Debugger_TraceMemory },
            { m_cmbTraceGameFile, Debugger_TraceGameFile },
        };
        for (size_t i = 0, n = sizeof(TraceCMB) / sizeof(TraceCMB[0]); i < n; i++)
        {
            g_Settings->SaveDword(TraceCMB[i].SettingId, (uint32_t)(TraceCMB[i].cmb.GetItemData(TraceCMB[i].cmb.GetCurSel())));
        }
        CSettingTypeApplication::Flush();
        return true;
    }
private:
    void ItemChanged(UINT /*Code*/, int /*id*/, HWND /*ctl*/)
    {
        SendMessage(GetParent(), PSM_CHANGED, (WPARAM)m_hWnd, 0);
    }

    CComboBox m_cmbTraceAppInit;
    CComboBox m_cmbTraceAppCleanup;
    CComboBox m_cmbTraceServiceCall;
    CComboBox m_cmbTraceHleKernel;
    CComboBox m_cmbTraceMemory;
    CComboBox m_cmbTraceGameFile;
};

COptionsSheet::COptionsSheet(const wchar_t* title, UINT /*uStartPage*/, HWND /*hWndParent*/) :
    m_pgLogSettings(new CLogSettings),
    m_Title(title)
{
    SetTitle(m_Title.c_str());
    AddPage(&m_pgLogSettings->m_psp);
}

COptionsSheet::~COptionsSheet()
{
    delete m_pgLogSettings;
}

void CDebuggerUI::EditLoggingOptions(void * hWndParent)
{
    COptionsSheet(L"Log Settings").DoModal((HWND)hWndParent);
}
