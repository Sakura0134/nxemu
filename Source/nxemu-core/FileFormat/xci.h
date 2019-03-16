#pragma once
#include <Common\path.h>
#include <nxemu-core\Machine\SwitchSystem.h>

class CXci
{
public:
	CXci(CSwitchKeys & Keys, const CPath & File);
	~CXci();

	static bool IsXciFile(const CPath & XciFile);
	inline bool Valid(void) const { return m_Valid; }

private:
	CXci(void);                    // Disable default constructor
	CXci(const CXci&);             // Disable copy constructor
    CXci& operator=(const CXci&);  // Disable assignment
		
	struct GamecardHeader
	{
		uint8_t signature[0x100];
		uint32_t Magic;
		uint32_t SecureAreaStartAddress;
		uint32_t BackupAreaStartAddress;
		uint8_t TitleKEKIndex;
		uint8_t GamecardSize;
		uint8_t GamecardHeaderVersion;
		uint8_t Flags;
		uint64_t PackageId;
		uint64_t ValidDataEndAddress;
		uint8_t GamecardInfoIV[0x10];
		uint64_t HFS0PartitionOffset;
		uint64_t HFS0HeaderSize;
		uint8_t HFS0HeaderHash[0x20];
		uint8_t InitialDataHash[0x20];
		uint32_t SecureModeFlag;
		uint32_t TitleKeyFlag;
		uint32_t KeyFlag;
		uint32_t NormalAreaEndAddress;
		uint8_t GamecardInfo[0x70];
	};
	bool m_Valid;
};

