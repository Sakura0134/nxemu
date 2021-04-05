#pragma once
#include <nxemu-core\FileSystem\PartitionFilesystem.h>
#include <nxemu-core\FileSystem\EncryptedFile.h>
#include <stdint.h>
#include <vector>

class CNMT
{
public:
	enum ContentRecordType
	{
		Meta = 0,
		Program = 1,
		Data = 2,
		Control = 3,
		Manual = 4,
		Legal = 5,
		Patch = 6,
	};

	enum TitleType
	{
		SystemProgram = 0x01,
		SystemDataArchive = 0x02,
		SystemUpdate = 0x03,
		FirmwarePackageA = 0x04,
		FirmwarePackageB = 0x05,
		Application = 0x80,
		Update = 0x81,
		AOC = 0x82,
		DeltaTitle = 0x83,
	};

	struct ContentRecord
	{
		uint8_t hash[0x20];
		uint8_t nca_id[0x10];
		uint8_t size[0x6];
		uint8_t type;
		uint8_t padding[0x1];
	};

	typedef std::vector<ContentRecord> ContentRecords;

	CNMT(CEncryptedFile & EncryptedFile, int64_t Offset, const CPartitionFilesystem::VirtualFile & File);

	inline uint64_t GetTitleID() const { return m_Header.title_id; }
	const ContentRecords & GetContentRecords() const { return m_ContentRecords; }

private:
	CNMT(void);                    // Disable default constructor
	CNMT(const CNMT&);             // Disable copy constructor
	CNMT& operator=(const CNMT&);  // Disable assignment

	struct Header
	{
		uint64_t title_id;
		uint32_t title_version;
		uint8_t type;
		uint8_t padding;
		uint16_t table_offset;
		uint16_t number_content_entries;
		uint16_t number_meta_entries;
		uint8_t padding2[12];
	};

	struct OptionalHeader 
	{
		uint64_t title_id;
		uint64_t minimum_version;
	};

	Header m_Header;
	OptionalHeader m_OptHeader;
	ContentRecords m_ContentRecords;
};