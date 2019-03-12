#pragma once
#include <Common\stdtypes.h>
#include <Common\IniFileClass.h>
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
		MasterKey,
		AreaKey,
	};

	typedef std::vector<uint8_t> KeyData;
	typedef std::pair<KeyType, uint32_t> KeyTypeIndex;
	typedef std::map<KeyType, KeyData> Keys;
	typedef std::map<KeyTypeIndex, KeyData> KeysIndex;

	CSwitchKeys();
	~CSwitchKeys();

	void LoadKeys(Keys & keys);
	void LoadKeysIndex(KeysIndex & keys);

	static bool ValidKey(KeyType type, const KeyData & key);
	static bool ValidKeyIndex(KeyType type, uint32_t index, const KeyData & key);

private:
	CSwitchKeys(const CSwitchKeys&);				// Disable copy constructor
	CSwitchKeys& operator=(const CSwitchKeys&);		// Disable assignment
	KeyData KeyValueData (const std::string &KeyValue);
	CIniFile m_KeyFile;
};