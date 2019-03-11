#include <nxemu\UserInterface\SwitchKeysConfig.h>
#include <nxemu-core\SystemGlobals.h>

CKeysConfig::CKeysConfig(void)
{
}

CKeysConfig::~CKeysConfig(void)
{
}

bool CKeysConfig::Display(void * ParentWindow)
{
	_Module.AddCreateWndData(&m_thunk.cd, this);
	if (ParentWindow != NULL && ::IsWindowVisible((HWND)ParentWindow) == 0)
	{
		ParentWindow = NULL;
	}
	INT_PTR result = ::DialogBoxParamW(NULL, MAKEINTRESOURCEW(IDD), (HWND)ParentWindow, StartDialogProc, 0);
	return result == IDOK;
}

LRESULT	CKeysConfig::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CKeysConfig::OnOkCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CKeysConfig::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

INT_PTR CALLBACK CKeysConfig::StartDialogProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	CKeysConfig * pThis = (CKeysConfig *)_Module.ExtractCreateWndData();
	ATLASSERT(pThis != NULL);
	if (!pThis)
	{
		return 0;
	}
	pThis->m_hWnd = hWnd;
#ifdef _DEBUG
	pThis->m_bModal = TRUE;
#endif
	pThis->m_thunk.Init((WNDPROC)pThis->GetDialogProc(), pThis);
	DLGPROC pProc = (DLGPROC)pThis->m_thunk.GetWNDPROC();
	DLGPROC pOldProc = (DLGPROC)::SetWindowLongPtrW(hWnd, DWLP_DLGPROC, (LONG_PTR)pProc);
#ifdef _DEBUG
	// check if somebody has subclassed us already since we discard it
	if (pOldProc != StartDialogProc)
		ATLTRACE(atlTraceWindowing, 0, _T("Subclassing through a hook discarded.\n"));
#else
	DBG_UNREFERENCED_LOCAL_VARIABLE(pOldProc);	// avoid unused warning
#endif
	return pProc(hWnd, uMsg, wParam, lParam);
}

