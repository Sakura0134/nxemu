#include <nxemu\UserInterface\SwitchKeysConfig.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\path.h>
#include <Common\StdString.h>

CKeysConfig::CKeysConfig(CSwitchKeys * keys) :
	m_keys(keys)
{
}

CKeysConfig::~CKeysConfig(void)
{
}

bool CKeysConfig::Display(void * ParentWindow)
{
	if (m_keys == NULL)
	{
		return false;
	}
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
		{ CSwitchKeys::TitlekekSource, IDC_TITLE_KEK_SOURCE },
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

LRESULT CKeysConfig::OnLoadCmd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CPath FileName;
    const char * Filter = "Key File (*.keys)\0*.keys\0All files (*.*)\0*.*\0";
    if (!FileName.SelectFile(m_hWnd, CPath(CPath::MODULE_DIRECTORY), Filter, true))
    {
        return 0;
    }
    CIniFile KeyIniFile(FileName);
    if (!KeyIniFile.IsFileOpen())
    {
        return 0;
    }

    CIniFile::KeyValueData data;
    KeyIniFile.GetKeyValueData(NULL, data);

    struct
    {
        CSwitchKeys::KeyType type;
        int nId;
        const char * key;
    }
    KeyItems[] =
    {
        { CSwitchKeys::HeaderKey, IDC_HEADER_KEY, "header_key" },
        { CSwitchKeys::KeyAreaKeyApplicationSource, IDC_KEY_AREA_KEY_APPLICATION_SOURCE, "key_area_key_application_source" },
        { CSwitchKeys::AesKeyGenerationSource, IDC_AES_KEY_GENERATION_SOURCE, "aes_key_generation_source" },
        { CSwitchKeys::AesKekGenerationSource, IDC_AES_KEK_GENERATION_SOURCE, "aes_kek_generation_source" },
        { CSwitchKeys::TitlekekSource, IDC_TITLE_KEK_SOURCE, "titlekek_source" },
    };

    CSwitchKeys::Keys keys;
    for (size_t i = 0, n = sizeof(KeyItems) / sizeof(KeyItems[0]); i < n; i++)
    {
        CIniFile::KeyValueData::const_iterator itr = data.find(KeyItems[i].key);
        if (itr == data.end())
        { 
            continue;
        }
        SetDlgItemText(KeyItems[i].nId, stdstr(itr->second).Trim().ToUTF16().c_str());
        if (!AddKey(keys, KeyItems[i].type, KeyItems[i].nId)) 
        {
            SetDlgItemText(KeyItems[i].nId, L"");
        }
    }

    struct
    {
        CSwitchKeys::KeyType type;
        int nId;
        uint32_t index;
        const char * key;
    } KeyIndexItems[] =
    {
        { CSwitchKeys::MasterKey, IDC_MASTER_KEY_00, 0, "master_key_00" },
        { CSwitchKeys::MasterKey, IDC_MASTER_KEY_01, 1, "master_key_01" },
        { CSwitchKeys::MasterKey, IDC_MASTER_KEY_02, 2, "master_key_02" },
        { CSwitchKeys::MasterKey, IDC_MASTER_KEY_03, 3, "master_key_03" },
        { CSwitchKeys::MasterKey, IDC_MASTER_KEY_04, 4, "master_key_04" },
        { CSwitchKeys::MasterKey, IDC_MASTER_KEY_05, 5, "master_key_05" },
    };

    CSwitchKeys::KeysIndex keysIndex;
    for (size_t i = 0, n = sizeof(KeyIndexItems) / sizeof(KeyIndexItems[0]); i < n; i++)
    {
        CIniFile::KeyValueData::const_iterator itr = data.find(KeyIndexItems[i].key);
        if (itr == data.end())
        {
            continue;
        }
        SetDlgItemText(KeyIndexItems[i].nId, stdstr(itr->second).Trim().ToUTF16().c_str());
        if (!AddKeyIndex(keysIndex, KeyIndexItems[i].type, KeyIndexItems[i].index, KeyIndexItems[i].nId))
        {
            SetDlgItemText(KeyItems[i].nId, L"");
        }
    }

    return 0;
}

LRESULT CKeysConfig::OnOkCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	struct 
	{
		CSwitchKeys::KeyType type;
		int nId;
		const char * message;
	} 
	KeyItems[] =
	{
		{ CSwitchKeys::HeaderKey, IDC_HEADER_KEY, "Header Key Invalid" },
		{ CSwitchKeys::KeyAreaKeyApplicationSource, IDC_KEY_AREA_KEY_APPLICATION_SOURCE, "key area key application source Invalid" },
		{ CSwitchKeys::AesKeyGenerationSource, IDC_AES_KEY_GENERATION_SOURCE, "Aes Key Generation Source Invalid" },
		{ CSwitchKeys::AesKekGenerationSource, IDC_AES_KEK_GENERATION_SOURCE, "Aes Kek Generation Source Invalid" },
		{ CSwitchKeys::TitlekekSource, IDC_TITLE_KEK_SOURCE, "Title Kek Source Invalid" },
	};

	struct 
	{
		CSwitchKeys::KeyType type;
		int nId;
		uint32_t index;
		const char * message;
	} KeyIndexItems[] =
	{
		{ CSwitchKeys::MasterKey, IDC_MASTER_KEY_00, 0, "Master Key 0 Invalid" },
		{ CSwitchKeys::MasterKey, IDC_MASTER_KEY_01, 1, "Master Key 1 Invalid" },
		{ CSwitchKeys::MasterKey, IDC_MASTER_KEY_02, 2, "Master Key 2 Invalid" },
		{ CSwitchKeys::MasterKey, IDC_MASTER_KEY_03, 3, "Master Key 3 Invalid" },
		{ CSwitchKeys::MasterKey, IDC_MASTER_KEY_04, 4, "Master Key 4 Invalid" },
		{ CSwitchKeys::MasterKey, IDC_MASTER_KEY_05, 5, "Master Key 5 Invalid" },
	};

	bool AllValid = true;
	CSwitchKeys::Keys keys;
	CSwitchKeys::KeysIndex keysIndex;

	for (size_t i = 0, n = sizeof(KeyItems) / sizeof(KeyItems[0]); i < n; i++)
	{
		if (!AddKey(keys, KeyItems[i].type, KeyItems[i].nId)) { AllValid = false; }
	}

	for (size_t i = 0, n = sizeof(KeyIndexItems) / sizeof(KeyIndexItems[0]); i < n; i++)
	{
		if (!AddKeyIndex(keysIndex, KeyIndexItems[i].type, KeyIndexItems[i].index, KeyIndexItems[i].nId)) { AllValid = false; }
	}

	if (AllValid)
	{
		if (!m_keys->SaveKeys(keys) || !m_keys->SaveKeysIndex(keysIndex))
		{
			g_Notify->DisplayError("Failed to save keys to file");
			return 0;
		}
		EndDialog(wID);
		return 0;
	}

	for (size_t i = 0, n = sizeof(KeyItems) / sizeof(KeyItems[0]); i < n; i++)
	{
		if (!ValidKey(KeyItems[i].type, KeyItems[i].nId))
		{
			g_Notify->DisplayError(KeyItems[i].message);
			GetDlgItem(KeyItems[i].nId).SetFocus();
			return 0;
		}
	}

	for (size_t i = 0, n = sizeof(KeyIndexItems) / sizeof(KeyIndexItems[0]); i < n; i++)
	{
		if (!ValidKeyIndex(KeyIndexItems[i].type, KeyIndexItems[i].index, KeyIndexItems[i].nId))
		{
			g_Notify->DisplayError(KeyIndexItems[i].message);
			GetDlgItem(KeyIndexItems[i].nId).SetFocus();
			return 0;
		}
	}
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

bool CKeysConfig::GetKeyData(int nID, CSwitchKeys::KeyData &data)
{
	std::wstring key;
	CWindow DlgItem = GetDlgItem(nID);
	ATLASSERT(::IsWindow(DlgItem));

	int nLen = ::GetWindowTextLengthW(DlgItem);
	if (nLen == 0)
	{
		return false;
	}
	key.resize(nLen + 1);
	::GetWindowTextW(DlgItem, (wchar_t *)key.c_str(), nLen + 1);

	if (nLen % 2 != 0)
	{
		return false;
	}

	for (size_t i = 0; i < nLen; i += 2)
	{
		uint8_t value = 0;
		if (key[i] >= L'A' && key[i] <= L'F')
		{
			value = (uint8_t)((10 + (key[i] - L'A')) << 4);
		}
		else if (key[i] >= L'a' && key[i] <= L'f')
		{
			value = (uint8_t)((10 + (key[i] - L'a')) << 4);
		}
		else if (key[i] >= L'0' && key[i] <= L'9')
		{
			value = (uint8_t)((key[i] - L'0') << 4);
		}
		else
		{
			return false;
		}

		if (key[i + 1] >= L'A' && key[i + 1] <= L'F')
		{
			value |= (10 + (key[i + 1] - L'A'));
		}
		else if (key[i + 1] >= L'a' && key[i + 1] <= L'f')
		{
			value |= (10 + (key[i + 1] - L'a'));
		}
		else if (key[i + 1] >= L'0' && key[i + 1] <= L'9')
		{
			value |= (key[i + 1] - L'0');
		}
		else
		{
			return false;
		}
		data.push_back(value);
	}
	return true;
}

bool CKeysConfig::AddKey(CSwitchKeys::Keys & keys, CSwitchKeys::KeyType type, int nID)
{
	CSwitchKeys::KeyData data;
	if (!GetKeyData(nID, data))
	{
		return false;
	}

	if (!CSwitchKeys::ValidKey(type, data))
	{
		return false;
	}
	keys.insert(CSwitchKeys::Keys::value_type(type, data));
	return true;
}

bool CKeysConfig::AddKeyIndex(CSwitchKeys::KeysIndex & keys, CSwitchKeys::KeyType type, uint32_t index, int nID)
{
	typedef CSwitchKeys::KeysIndex KeysIndex;
	typedef CSwitchKeys::KeyTypeIndex KeyTypeIndex;
	typedef CSwitchKeys::KeyData KeyData;

	KeyData data;
	if (!GetKeyData(nID, data))
	{
		return false;
	}

	if (!CSwitchKeys::ValidKeyIndex(type, index, data))
	{
		return false;
	}
	keys.insert(KeysIndex::value_type(KeyTypeIndex(type, index), data));
	return true;
}

bool CKeysConfig::ValidKey(CSwitchKeys::KeyType type, int nID)
{
	CSwitchKeys::KeyData data;
	if (!GetKeyData(nID, data))
	{
		return false;
	}
	return CSwitchKeys::ValidKey(type, data);
}

bool CKeysConfig::ValidKeyIndex(CSwitchKeys::KeyType type, uint32_t index, int nID)
{
	CSwitchKeys::KeyData data;
	if (!GetKeyData(nID, data))
	{
		return false;
	}
	return CSwitchKeys::ValidKeyIndex(type, index, data);
}

void CKeysConfig::SetKeyData(int nID, const CSwitchKeys::KeyData &data)
{
	std::wstring KeyValue;
	for (size_t i = 0, n = data.size(); i < n; i++)
	{
		wchar_t value[3];
		_swprintf(value, L"%02X", data[i]);
		KeyValue += value;
	}
	SetDlgItemText(nID, KeyValue.c_str());
}
