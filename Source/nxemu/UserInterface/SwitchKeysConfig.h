#pragma once
#include <nxemu\UserInterface\WTLApp.h>
#include <nxemu-core\Machine\SwitchKeys.h>

class CKeysConfig :
	public CDialogImpl < CKeysConfig >
{
public:
	BEGIN_MSG_MAP_EX(CKeysConfig)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOkCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	enum { IDD = IDD_Settings_keys };

	CKeysConfig(CSwitchKeys * keys);
	~CKeysConfig(void);

	bool Display(void * ParentWindow);

	LRESULT	OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOkCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	bool AddKey(CSwitchKeys::Keys & keys, CSwitchKeys::KeyType type, int nID);
	bool AddKeyIndex(CSwitchKeys::KeysIndex & keys, CSwitchKeys::KeyType type, uint32_t index, int nID);
	bool ValidKey(CSwitchKeys::KeyType type, int nID);
	bool ValidKeyIndex(CSwitchKeys::KeyType type, uint32_t index, int nID);
	bool GetKeyData(int nID, CSwitchKeys::KeyData &data);
	void SetKeyData(int nID, const CSwitchKeys::KeyData &data);

	static INT_PTR CALLBACK StartDialogProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	CSwitchKeys * m_keys;
};
