#include <nxemu-core\Machine\SwitchKeys.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Settings\Settings.h>
#include <Common\sha256.h>

CSwitchKeys::CSwitchKeys() :
	m_KeyFile(g_Settings->LoadStringVal(SupportFile_Keys).c_str())
{
}

CSwitchKeys::~CSwitchKeys()
{
}

void CSwitchKeys::LoadKeys(Keys & keys)
{
	struct
	{
		KeyType type;
		const char * key;
	}
	Items[] =
	{
		{ HeaderKey,"header_key" },
		{ KeyAreaKeyApplicationSource, "key_area_key_application_source" },
		{ AesKekGenerationSource, "aes_key_generation_source" },
		{ AesKeyGenerationSource, "aes_kek_generation_source" },
	};

	CIniFile::strlist IniKeys;
	m_KeyFile.GetKeyList("", IniKeys);
	for (CIniFile::strlist::const_iterator itr = IniKeys.begin(); itr != IniKeys.end(); itr++)
	{
		for (size_t i = 0, n = (sizeof(Items) / sizeof(Items[0])); i < n; i++)
		{
			if (_stricmp(itr->c_str(), Items[i].key) == 0)
			{
				std::string KeyValue = m_KeyFile.GetString("", Items[i].key, "");
				KeyData data = KeyValueData(KeyValue);
				if (ValidKey(Items[i].type, data))
				{
					keys.insert(Keys::value_type(Items[i].type, data));
				}
			}
		}
	}
}

void CSwitchKeys::LoadKeysIndex(KeysIndex & keys)
{
	struct
	{
		KeyType type;
		uint32_t index;
		const char * key;
	}
	Items[] =
	{
		{ MasterKey, 0, "master_key_00" },
		{ MasterKey, 1, "master_key_01" },
		{ MasterKey, 2, "master_key_02" },
		{ MasterKey, 3, "master_key_03" },
		{ MasterKey, 4, "master_key_04" },
		{ MasterKey, 5, "master_key_05" },
	};

	CIniFile::strlist IniKeys;
	m_KeyFile.GetKeyList("", IniKeys);
	for (CIniFile::strlist::const_iterator itr = IniKeys.begin(); itr != IniKeys.end(); itr++)
	{
		for (size_t i = 0, n = (sizeof(Items) / sizeof(Items[0])); i < n; i++)
		{
			if (_stricmp(itr->c_str(), Items[i].key) == 0)
			{
				std::string KeyValue = m_KeyFile.GetString("", Items[i].key, "");
				KeyData data = KeyValueData(KeyValue);
				if (ValidKeyIndex(Items[i].type, Items[i].index, data))
				{
					keys.insert(KeysIndex::value_type(KeyTypeIndex(Items[i].type, Items[i].index), data));
				}
			}
		}
	}
}

bool CSwitchKeys::SaveKeys(const Keys & keys)
{
	struct
	{
		KeyType type;
		const char * key;
	}
	Items[] =
	{
		{ HeaderKey,"header_key" },
		{ KeyAreaKeyApplicationSource, "key_area_key_application_source"},
		{ AesKekGenerationSource, "aes_key_generation_source" },
		{ AesKeyGenerationSource, "aes_kek_generation_source" },
	};

	for (size_t i = 0, n = sizeof(Items) / sizeof(Items[0]); i < n; i++)
	{
		Keys::const_iterator itr = keys.find(Items[i].type);
		if (itr == keys.end())
		{
			return false;
		}
		const KeyData & data = itr->second;
		if (!SaveKey(m_KeyFile, data, Items[i].key))
		{
			return false;
		}
	}
    return true;
}

bool CSwitchKeys::SaveKeysIndex(const KeysIndex & keys)
{
	struct
	{
		KeyType type;
		uint32_t index;
		const char * key;
	}
	Items[] =
	{
		{ MasterKey, 0, "master_key_00" },
		{ MasterKey, 1, "master_key_01" },
		{ MasterKey, 2, "master_key_02" },
		{ MasterKey, 3, "master_key_03" },
		{ MasterKey, 4, "master_key_04" },
		{ MasterKey, 5, "master_key_05" },
	};

	for (size_t i = 0, n = sizeof(Items) / sizeof(Items[0]); i < n; i++)
	{
		KeysIndex::const_iterator itr = keys.find(KeyTypeIndex(Items[i].type, Items[i].index));
		if (itr != keys.end())
		{
			const KeyData & data = itr->second;
			if (!SaveKey(m_KeyFile, data, Items[i].key))
			{
				return false;
			}
		}
	}
	return true;
}

bool CSwitchKeys::ValidKey(KeyType type, const KeyData & key)
{
	struct 
	{
		KeyType type;
		uint8_t sha256[SHA256::DIGEST_SIZE];
	} 
	ValidItems[] =
	{
		{ HeaderKey,{ 0x8E, 0x03, 0xDE, 0x24, 0x81, 0x8D, 0x96, 0xCE, 0x4F, 0x2A, 0x09, 0xB4, 0x3A, 0xF9, 0x79, 0xE6, 0x79, 0x97, 0x4F, 0x75, 0x70, 0x71, 0x3A, 0x61, 0xEE, 0xD8, 0xB3, 0x14, 0x86, 0x4A, 0x11, 0xD5 } },
		{ KeyAreaKeyApplicationSource,{ 0x04, 0xAD, 0x66, 0x14, 0x3C, 0x72, 0x6B, 0x2A, 0x13, 0x9F, 0xB6, 0xB2, 0x11, 0x28, 0xB4, 0x6F, 0x56, 0xC5, 0x53, 0xB2, 0xB3, 0x88, 0x71, 0x10, 0x30, 0x42, 0x98, 0xD8, 0xD0, 0x09, 0x2D, 0x9E }},
		{ AesKekGenerationSource,{ 0xFC, 0x02, 0xB9, 0xD3, 0x7B, 0x42, 0xD7, 0xA1, 0x45, 0x2E, 0x71, 0x44, 0x4F, 0x1F, 0x70, 0x03, 0x11, 0xD1, 0x13, 0x2E, 0x30, 0x1A, 0x83, 0xB1, 0x60, 0x62, 0xE7, 0x2A, 0x78, 0x17, 0x50, 0x85 }},
		{ AesKeyGenerationSource,{ 0xFB, 0xD1, 0x00, 0x56, 0x99, 0x9E, 0xDC, 0x7A, 0xCD, 0xB9, 0x60, 0x98, 0xE4, 0x7E, 0x2C, 0x36, 0x06, 0x23, 0x02, 0x70, 0xD2, 0x32, 0x81, 0xE6, 0x71, 0xF0, 0xF3, 0x89, 0xFC, 0x5B, 0xC5, 0x85 }},
	};

	unsigned char digest[SHA256::DIGEST_SIZE] = { 0 };
	SHA256 ctx = SHA256();
	ctx.init();
	ctx.update(key.data(), (unsigned int)key.size());
	ctx.final(digest);

	for (size_t i = 0, n = sizeof(ValidItems) / sizeof(ValidItems[0]); i < n; i++)
	{
		if (type == ValidItems[i].type)
		{
			if (memcmp(ValidItems[i].sha256, digest, sizeof(digest) / sizeof(digest[0])) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool CSwitchKeys::ValidKeyIndex(KeyType type, uint32_t index, const KeyData & key)
{
	struct
	{
		KeyType type;
		uint32_t index;
		uint8_t sha256[SHA256::DIGEST_SIZE];
	}
	ValidItems[] =
	{
		{ MasterKey, 0, { 0x0E, 0xE3, 0x59, 0xBE, 0x3C, 0x86, 0x4B, 0xB0, 0x78, 0x2E, 0x1D, 0x70, 0xA7, 0x18, 0xA0, 0x34, 0x2C, 0x55, 0x1E, 0xED, 0x28, 0xC3, 0x69, 0x75, 0x4F, 0x9C, 0x4F, 0x69, 0x1B, 0xEC, 0xF7, 0xCA }},
		{ MasterKey, 1, { 0x4F, 0xE7, 0x07, 0xB7, 0xE4, 0xAB, 0xDA, 0xF7, 0x27, 0xC8, 0x94, 0xAA, 0xF1, 0x3B, 0x13, 0x51, 0xBF, 0xE2, 0xAC, 0x90, 0xD8, 0x75, 0xF7, 0x3B, 0x2E, 0x20, 0xFA, 0x94, 0xB9, 0xCC, 0x66, 0x1E }},
		{ MasterKey, 2, { 0x79, 0x27, 0x7C, 0x02, 0x37, 0xA2, 0x25, 0x2E, 0xC3, 0xDF, 0xAC, 0x1F, 0x7C, 0x35, 0x9C, 0x2B, 0x3D, 0x12, 0x1E, 0x9D, 0xB1, 0x5B, 0xB9, 0xAB, 0x4C, 0x2B, 0x44, 0x08, 0xD2, 0xF3, 0xAE, 0x09 }},
		{ MasterKey, 3, { 0x4F, 0x36, 0xC5, 0x65, 0xD1, 0x33, 0x25, 0xF6, 0x5E, 0xE1, 0x34, 0x07, 0x3C, 0x6A, 0x57, 0x8F, 0xFC, 0xB0, 0x00, 0x8E, 0x02, 0xD6, 0x94, 0x00, 0x83, 0x68, 0x44, 0xEA, 0xB7, 0x43, 0x27, 0x54 }},
		{ MasterKey, 4, { 0x75, 0xFF, 0x1D, 0x95, 0xD2, 0x61, 0x13, 0x55, 0x0E, 0xE6, 0xFC, 0xC2, 0x0A, 0xCB, 0x58, 0xE9, 0x7E, 0xDE, 0xB3, 0xA2, 0xFF, 0x52, 0x54, 0x3E, 0xD5, 0xAE, 0xC6, 0x3B, 0xDC, 0xC3, 0xDA, 0x50 }},
		{ MasterKey, 5, { 0xEB, 0xE2, 0xBC, 0xD6, 0x70, 0x46, 0x73, 0xEC, 0x0F, 0x88, 0xA1, 0x87, 0xBB, 0x2A, 0xD9, 0xF1, 0xCC, 0x82, 0xB7, 0x18, 0xC3, 0x89, 0x42, 0x59, 0x41, 0xBD, 0xC1, 0x94, 0xDC, 0x46, 0xB0, 0xDD }},
	};

	unsigned char digest[SHA256::DIGEST_SIZE] = { 0 };
	SHA256 ctx = SHA256();
	ctx.init();
	ctx.update(key.data(), (unsigned int)key.size());
	ctx.final(digest);

	for (size_t i = 0, n = sizeof(ValidItems) / sizeof(ValidItems[0]); i < n; i++)
	{
		if (type == ValidItems[i].type && ValidItems[i].index == index)
		{
			if (memcmp(ValidItems[i].sha256, digest, sizeof(digest) / sizeof(digest[0])) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool CSwitchKeys::SaveKey(CIniFile & KeyFile, const KeyData & data, const char * name)
{
	std::string KeyValue;
	for (size_t i = 0, n = data.size(); i < n; i++)
	{
		char value[3];
		sprintf(value, "%02X", data[i]);
		KeyValue += value;
	}
	KeyFile.SaveString("", name, KeyValue.c_str());
	return true;
}

CSwitchKeys::KeyData CSwitchKeys::KeyValueData(const std::string &KeyValue)
{
	KeyData data;
	for (size_t i = 0, nLen = KeyValue.length(); i < nLen; i += 2)
	{
		uint8_t value = 0;
		if (KeyValue[i] >= 'A' && KeyValue[i] <= 'F')
		{
			value = (10 + (KeyValue[i] - 'A')) << 4;
		}
		else if (KeyValue[i] >= 'a' && KeyValue[i] <= 'f')
		{
			value = (10 + (KeyValue[i] - 'a')) << 4;
		}
		else if (KeyValue[i] >= '0' && KeyValue[i] <= '9')
		{
			value = (KeyValue[i] - '0') << 4;
		}
		else
		{
			break;
		}

		if (KeyValue[i + 1] >= 'A' && KeyValue[i + 1] <= 'F')
		{
			value |= (10 + (KeyValue[i + 1] - 'A'));
		}
		else if (KeyValue[i + 1] >= 'a' && KeyValue[i + 1] <= 'f')
		{
			value |= (10 + (KeyValue[i + 1] - 'a'));
		}
		else if (KeyValue[i + 1] >= '0' && KeyValue[i + 1] <= '9')
		{
			value |= (KeyValue[i + 1] - '0');
		}
		else
		{
			break;
		}
		data.push_back(value);
	}
	return data;
}

