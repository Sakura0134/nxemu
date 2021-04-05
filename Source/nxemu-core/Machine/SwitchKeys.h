#pragma once
#include <Common\IniFileClass.h>
#include <stdint.h>
#include <vector>
#include <map>

class CSwitchKeys
{
public:
	enum KeyType
	{
		UnknownKey,
		HeaderKey,
		KeyAreaKeyApplicationSource,
		AesKeyGenerationSource,
		AesKekGenerationSource,
		TitlekekSource,
		MasterKey,
		AreaKey,
		TitlekekKey,
	};

	typedef std::vector<uint8_t> KeyData;
	typedef std::pair<KeyType, uint32_t> KeyTypeIndex;
	typedef std::map<KeyType, KeyData> Keys;
	typedef std::map<KeyTypeIndex, KeyData> KeysIndex;
	typedef std::vector<uint8_t> TitleId;
	typedef std::map<TitleId, KeyData> KeysTitle;

	CSwitchKeys();
	~CSwitchKeys();

	void LoadKeys(Keys & keys);
	void LoadKeysIndex(KeysIndex & keys);
	bool SaveKeys(const Keys & keys);
	bool SaveKeysIndex(const KeysIndex & keys);
	bool GetKeyIndex(KeyType Type, uint32_t KeyIndex, KeyData & key);
	bool GetKey(KeyType Type, KeyData & key);
	
	void SetTitleKey(const uint8_t * id, size_t size, KeyData & key);
	bool GetTitleKey(const uint8_t * id, size_t size, KeyData & key);

	static bool ValidKey(KeyType type, const KeyData & key);
	static bool ValidKeyIndex(KeyType type, uint32_t index, const KeyData & key);

private:
	CSwitchKeys(const CSwitchKeys&);				// Disable copy constructor
	CSwitchKeys& operator=(const CSwitchKeys&);		// Disable assignment

	static bool GenerateKek(KeyData & key, const KeyData & Source, const KeyData & MasterKey, const KeyData & KekSeed, const KeyData & KeySeed);
	static bool SaveKey(CIniFile & KeyFile, const KeyData & data, const char * name);
	
	KeyData KeyValueData (const std::string &KeyValue);
	CIniFile m_KeyFile;
	KeysTitle m_TitleKeys;
};