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
	typedef CSwitchKeys::KeyTypeIndex KeyTypeIndex;
	typedef CSwitchKeys::KeyType KeyType;
	typedef CSwitchKeys::KeysIndex KeysIndex;
	typedef CSwitchKeys::Keys Keys;

	CSwitchKeys SwitchKeys;
	Keys keys;
	SwitchKeys.LoadKeys(keys);

	struct
	{
		KeyType type;
		int nId;
	}
	KeyItems[] =
	{
		{ CSwitchKeys::HeaderKey, IDC_HEADER_KEY },
		{ CSwitchKeys::KeyAreaKeyApplicationSource, IDC_KEY_AREA_KEY_APPLICATION_SOURCE },
		{ CSwitchKeys::AesKeyGenerationSource, IDC_AES_KEY_GENERATION_SOURCE },
		{ CSwitchKeys::AesKekGenerationSource, IDC_AES_KEK_GENERATION_SOURCE },
	};

	for (size_t i = 0, n = sizeof(KeyItems) / sizeof(KeyItems[0]); i < n; i++)
	{
		Keys::const_iterator itr = keys.find(KeyItems[i].type);
		if (itr != keys.end())
		{
			SetKeyData(KeyItems[i].nId, itr->second);
		}
	}

	KeysIndex keysIndex;
	SwitchKeys.LoadKeysIndex(keysIndex);
	struct
	{
		CSwitchKeys::KeyType type;
		uint32_t index;
		int nId;
	}
	KeyItemsIndex[] =
	{
		{ CSwitchKeys::MasterKey, 0, IDC_MASTER_KEY_00 },
		{ CSwitchKeys::MasterKey, 1, IDC_MASTER_KEY_01 },
		{ CSwitchKeys::MasterKey, 2, IDC_MASTER_KEY_02 },
		{ CSwitchKeys::MasterKey, 3, IDC_MASTER_KEY_03 },
		{ CSwitchKeys::MasterKey, 4, IDC_MASTER_KEY_04 },
		{ CSwitchKeys::MasterKey, 5, IDC_MASTER_KEY_05 },
	};

	for (size_t i = 0, n = sizeof(KeyItemsIndex) / sizeof(KeyItemsIndex[0]); i < n; i++)
	{
		KeysIndex::const_iterator itr = keysIndex.find(KeyTypeIndex(KeyItemsIndex[i].type, KeyItemsIndex[i].index));
		if (itr != keysIndex.end())
		{
			SetKeyData(KeyItemsIndex[i].nId, itr->second);
		}
	}
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


void CKeysConfig::SetKeyData(int nID, const CSwitchKeys::KeyData &data)
{
	std::wstring KeyValue;
	for (size_t i = 0, n = data.size(); i < n; i++)
	{
		wchar_t value[3];
		swprintf(value, L"%02X", data[i]);
		KeyValue += value;
	}
	SetDlgItemTextW(m_hWnd, nID, KeyValue.c_str());
}
