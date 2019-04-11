#pragma once
#include <nxemu-core\Machine\SwitchKeys.h>
#include <nxemu-core\FileSystem\EncryptedFile.h>
#include <Common\stdtypes.h>
#include <Common\FileClass.h>

class CPartitionFilesystem;

class NCA
{
public:
	typedef std::vector<CPartitionFilesystem*> CPartitionFilesystems;

	NCA(void);
	~NCA();

	bool Load(CSwitchKeys & Keys, CFile & file, int64_t BaseOffset, int64_t FileOffset, uint64_t Size);
	inline const CPartitionFilesystems & Dirs(void) const { return m_dirs; }
	inline CPartitionFilesystem * exefs(void) const { return m_exefs; }
	inline CEncryptedFile * Romfs(void) const { return m_Romfs; }
	inline uint64_t RomfsOffset(void) const { return m_RomfsOffset; }

private:
	NCA(const NCA&);            // Disable copy constructor
	NCA& operator=(const NCA&); // Disable assignment

	struct NCASectionTable
	{
		uint32_t media_offset;
		uint32_t media_end_offset;
		uint8_t padding2[0x8];
	};

	struct NCAHeader
	{
		uint8_t rsa_signature_1[0x100];
		uint8_t rsa_signature_2[0x100];
		uint8_t magic[4];
		uint8_t is_system;
		uint8_t content_type;
		uint8_t crypto_type;
		uint8_t key_index;
		uint64_t size;
		uint64_t title_id;
		uint32_t padding;
		uint32_t sdk_version;
		uint8_t crypto_type_2;
		uint8_t padding2[15];
		uint8_t rights_id[0x10];
		NCASectionTable section_tables[0x4];
		uint8_t hash_tables[0x80];
		uint8_t key_area[0x40];
		uint8_t decrypt_padding[0xC0];
	};

	enum class NCASectionFilesystemType : uint8_t
	{
		PFS0 = 0x2,
		ROMFS = 0x3,
	};

	enum class NCASectionCryptoType : uint8_t
	{
		NONE = 1,
		XTS = 2,
		CTR = 3,
		BKTR = 4,
	};

	struct NCASectionHeaderBlock
	{
		uint8_t padding[3];
		NCASectionFilesystemType filesystem_type;
		NCASectionCryptoType crypto_type;
		uint8_t padding1[3];
	};

	struct NCASectionRaw 
	{
		NCASectionHeaderBlock header;
		uint8_t block_data[0x138];
		uint8_t section_ctr[0x8];
		uint8_t padding[0xB8];
	};

	struct PFS0Superblock 
	{
		NCASectionHeaderBlock header_block;
		uint8_t hash[0x20];
		uint32_t size;
		uint8_t padding[4];
		uint64_t hash_table_offset;
		uint64_t hash_table_size;
		uint64_t pfs0_header_offset;
		uint64_t pfs0_size;
		uint8_t padding1[0x1B0];
	};

	struct IVFCLevel 
	{
		uint64_t offset;
		uint64_t size;
		uint32_t block_size;
		uint32_t reserved;
	};

	struct IVFCHeader
	{
		uint32_t magic;
		uint32_t magic_number;
		uint8_t padding[0x08];
		IVFCLevel levels[0x06];
		uint8_t padding1[0x40];
	};

	struct RomFSSuperblock 
	{
		NCASectionHeaderBlock header_block;
		IVFCHeader ivfc;
		uint8_t padding[0x118];
	};

	union NCASectionHeader 
	{
		NCASectionRaw raw;
		PFS0Superblock pfs0;
		RomFSSuperblock romfs;
	};

	bool ValidHeader(const NCAHeader & header);
	bool DecodeHeaderData(CSwitchKeys & Keys, uint8_t * Source, uint8_t * Dest, uint32_t size, size_t sector_id);
	bool ReadRomFSSection(CSwitchKeys & Keys, CFile & file, int64_t Offset, const NCASectionHeader & section, const NCASectionTable & entry);
	bool ReadPFS0Section(CSwitchKeys & Keys, CFile & file, int64_t Offset, const NCASectionHeader & section, const NCASectionTable & entry);
	bool SetupEncryptedFile(CEncryptedFile & EncryptedFile, CSwitchKeys & Keys, const NCASectionHeader & section, size_t StartOffset);
	bool GetTitleKey(CSwitchKeys & Keys, CSwitchKeys::KeyData & Key);
	uint8_t GetCryptoRevision() const;
	bool IsDirectoryExeFS(CPartitionFilesystem* dir);
	static const char * NCASectionFilesystemTypeName(NCASectionFilesystemType type);

	NCAHeader m_Header;
	bool m_Encrypted;
	bool m_HasRightsId;
	CPartitionFilesystems m_dirs;
	CPartitionFilesystem * m_exefs;
	CEncryptedFile * m_Romfs;
	uint64_t m_RomfsOffset;
};
