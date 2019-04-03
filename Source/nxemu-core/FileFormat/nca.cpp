#include <nxemu-core\FileFormat\nca.h>
#include <nxemu-core\Util\AESCipher.h>
#include <nxemu-core\FileSystem\PartitionFilesystem.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>

NCA::NCA() :
	m_Header({ 0 }),
	m_Encrypted(false),
	m_HasRightsId(false),
	m_exefs(NULL),
	m_Romfs(NULL),
	m_RomfsOffset(0)
{
}

NCA::~NCA()
{
	for (size_t i = 0; i < m_dirs.size(); i++)
	{
		delete m_dirs[i];
	}
	m_dirs.clear();
	if (m_Romfs != NULL)
	{
		delete m_Romfs;
		m_Romfs = NULL;
	}
}

bool NCA::Load(CSwitchKeys & Keys, CFile & file, int64_t BaseOffset, int64_t FileOffset, uint64_t Size)
{
	WriteTrace(TraceGameFile, TraceVerbose, "Start (FileOffset: 0x%I64u Size: 0x%I64u)", FileOffset, Size);
	if (!file.IsOpen() || file.Seek(BaseOffset + FileOffset, CFileBase::begin) < 0)
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to seek to file (BaseOffset: 0x%I64u FileOffset: 0x%I64u)", BaseOffset, FileOffset);
		WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
		return false;
	}
	if (!file.Read(&m_Header, sizeof(m_Header)))
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to read header");
		WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
		return false;
	}

	m_Encrypted = false;
	if (!ValidHeader(m_Header))
	{
		WriteTrace(TraceGameFile, TraceVerbose, "header is not valid (magic: \"%c%c%c%c\"), trying to decrypt it", m_Header.magic[0], m_Header.magic[1], m_Header.magic[2], m_Header.magic[3]);
		if (!DecodeHeaderData(Keys, (uint8_t *)&m_Header, (uint8_t *)&m_Header, sizeof(m_Header), 0))
		{
			WriteTrace(TraceGameFile, TraceError, "Failed to decode header data");
			WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
			return false;
		}
		if (!ValidHeader(m_Header))
		{
			WriteTrace(TraceGameFile, TraceError, "invalid header (magic: \"%c%c%c%c\")", m_Header.magic[0], m_Header.magic[1], m_Header.magic[2], m_Header.magic[3]);
			WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
			return false;
		}
		WriteTrace(TraceGameFile, TraceVerbose, "nca is encrypted");
		m_Encrypted = true;
	}
	WriteTrace(TraceGameFile, TraceVerbose, "header magic: \"%c%c%c%c\"", m_Header.magic[0], m_Header.magic[1], m_Header.magic[2], m_Header.magic[3]);

	for (size_t i = 0, n = sizeof(m_Header.rights_id) / sizeof(m_Header.rights_id[0]); i < n; i++)
	{
		if (m_Header.rights_id[i] != '\0')
		{
			m_HasRightsId = true;
			break;
		}
	}
	WriteTrace(TraceGameFile, TraceVerbose, "HasRightsId: %s", m_HasRightsId ? "True" : "False");

	uint32_t NoOfSections = 0;
	for (size_t i = 0, n = sizeof(m_Header.section_tables) / sizeof(m_Header.section_tables[0]); i < n; i++)
	{
		WriteTrace(TraceGameFile, TraceVerbose, "m_Header.section_tables[%d].media_offset=0x%X", i, m_Header.section_tables[i].media_offset);
		if (m_Header.section_tables[i].media_offset > 0)
		{
			NoOfSections += 1;
		}
	}
	WriteTrace(TraceGameFile, TraceVerbose, "NoOfSections: %d", NoOfSections);
	std::vector<NCASectionHeader> Sections(NoOfSections);
	if (!file.Read(&Sections[0], sizeof(NCASectionHeader) * NoOfSections))
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to read NCASectionHeader");
		WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
		return false;
	}
	if (m_Encrypted && !DecodeHeaderData(Keys, (uint8_t *)&Sections[0], (uint8_t *)&Sections[0], sizeof(NCASectionHeader) * NoOfSections, 2))
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to decrypt NCASectionHeader");
		WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
		return false;
	}
	for (size_t i = 0; i < NoOfSections; i++)
	{
		NCASectionHeader & section = Sections[i];
		WriteTrace(TraceGameFile, TraceVerbose, "Sections[%d]: filesystem_type = %s (%d)", i, NCASectionFilesystemTypeName(section.raw.header.filesystem_type), section.raw.header.filesystem_type);
		if (section.raw.header.filesystem_type == NCASectionFilesystemType::ROMFS)
		{
			if (!ReadRomFSSection(Keys, file, BaseOffset + FileOffset, section, m_Header.section_tables[i]))
			{
				return false;
			}
		}
		else if (section.raw.header.filesystem_type == NCASectionFilesystemType::PFS0)
		{
			if (!ReadPFS0Section(Keys, file, BaseOffset + FileOffset, section, m_Header.section_tables[i]))
			{
				return false;
			}
		}
		else
		{
			g_Notify->BreakPoint(__FILE__, __LINE__);
			return false;
		}
	}
	WriteTrace(TraceGameFile, TraceVerbose, "Done (res: true)");
	return true;
}

bool NCA::ValidHeader(const NCAHeader & header)
{
	return *((uint32_t *)&header.magic[0]) == *((uint32_t *)(&"NCA3"));
}

bool NCA::DecodeHeaderData(CSwitchKeys & Keys, uint8_t * Source, uint8_t * Dest, uint32_t size, size_t sector_id)
{
	CSwitchKeys::KeyData headr_key;
	if (!Keys.GetKey(CSwitchKeys::HeaderKey, headr_key))
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to get switch header key");
		return false;
	}

	CAESCipher cipher(headr_key.data(), (uint32_t)headr_key.size(), CAESCipher::CIPHER_AES_128_XTS);
	cipher.XTSTranscode(Source, Dest, size, sector_id, 0x200);
	return true;
}

bool NCA::ReadRomFSSection(CSwitchKeys & Keys, CFile & file, int64_t Offset, const NCASectionHeader & section, const NCASectionTable & entry)
{
	enum { MEDIA_OFFSET_MULTIPLIER = 0x200 };
	enum { IVFC_MAX_LEVEL = (sizeof(section.romfs.ivfc.levels) / sizeof(section.romfs.ivfc.levels[0])) };
	const uint64_t BaseOffset = ((uint64_t)entry.media_offset * MEDIA_OFFSET_MULTIPLIER);
	const uint64_t ivfc_offset = section.romfs.ivfc.levels[IVFC_MAX_LEVEL - 1].offset;
	const uint64_t romfs_offset = BaseOffset + ivfc_offset;
	const uint64_t romfs_size = section.romfs.ivfc.levels[IVFC_MAX_LEVEL - 1].size;

	std::auto_ptr<CEncryptedFile> EncryptedFile(new CEncryptedFile(file));
	if (!SetupEncryptedFile(*EncryptedFile.get(), Keys, section, romfs_offset))
	{
		return false;
	}
	
	if (section.raw.header.crypto_type == NCASectionCryptoType::BKTR)
	{
		return false;
	}
	else
	{
		if (m_Romfs != NULL)
		{
			g_Notify->BreakPoint(__FILE__, __LINE__);
		}
		m_Romfs = EncryptedFile.release();
		m_RomfsOffset = Offset + romfs_offset;
	}
	return true;
}

bool NCA::ReadPFS0Section(CSwitchKeys & Keys, CFile & file, int64_t Offset, const NCASectionHeader & section, const NCASectionTable & entry)
{
	WriteTrace(TraceGameFile, TraceVerbose, "Start (Offset: 0x%I64u)", Offset);
	enum { MEDIA_OFFSET_MULTIPLIER = 0x200 };
	uint64_t MediaOffset = ((uint64_t)entry.media_offset * MEDIA_OFFSET_MULTIPLIER) + section.pfs0.pfs0_header_offset;
	uint64_t MediaSize = MEDIA_OFFSET_MULTIPLIER * (entry.media_end_offset - entry.media_offset);
	WriteTrace(TraceGameFile, TraceVerbose, "MediaOffset: 0x%I64u MediaSize: 0x%I64u", MediaOffset, MediaSize);

	CEncryptedFile EncryptedFile(file);
	if (!SetupEncryptedFile(EncryptedFile, Keys, section, MediaOffset))
	{
		WriteTrace(TraceGameFile, TraceError, "failed to setup encrypted file");
		WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
		return false;
	}

	std::auto_ptr<CPartitionFilesystem> npfs(new CPartitionFilesystem(EncryptedFile, MediaOffset, Offset, MediaSize));
	if (!npfs->Valid())
	{
		WriteTrace(TraceGameFile, TraceError, "npfs is invalid");
		WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
		return false;
	}
	m_dirs.push_back(npfs.release());
	if (IsDirectoryExeFS(m_dirs.back()))
	{
		WriteTrace(TraceGameFile, TraceError, "directory is exe fs, setting m_exefs");
		m_exefs = m_dirs.back();
	}
	WriteTrace(TraceGameFile, TraceVerbose, "Done (res: true)");
	return true;
}

bool NCA::SetupEncryptedFile(CEncryptedFile & EncryptedFile, CSwitchKeys & Keys, const NCASectionHeader & section, size_t StartOffset)
{
	if (!m_Encrypted)
	{
		return true;
	}

	CSwitchKeys::KeyData Key;
	std::vector<uint8_t> iv(16);
	switch (section.raw.header.crypto_type)
	{
	case NCASectionCryptoType::NONE:
		return true;
	case NCASectionCryptoType::CTR:
	case NCASectionCryptoType::BKTR:
		if (m_HasRightsId)
		{
			if (!GetTitleKey(Keys, Key))
			{
				return false;
			}
		}
		else
		{
			CSwitchKeys::KeyData AreaKey;
			if (!Keys.GetKeyIndex(CSwitchKeys::AreaKey, GetCryptoRevision(), AreaKey))
			{
				g_Notify->BreakPoint(__FILE__, __LINE__);
				return false;
			}
			std::vector<uint8_t> key_area(sizeof(m_Header.key_area));
			CAESCipher cipher(AreaKey.data(), (uint32_t)AreaKey.size(), CAESCipher::CIPHER_AES_128_ECB);
			cipher.Transcode(m_Header.key_area, key_area.data(), (uint32_t)key_area.size());

			Key.resize(0x10);
			memcpy(Key.data(), &key_area[0x20], Key.size());
		}
		for (uint32_t i = 0; i < 8; i++)
		{
			iv[i] = section.raw.section_ctr[0x8 - i - 1];
		}
		EncryptedFile.SetEncrypted(Key.data(), Key.size(), iv, StartOffset);
		return true;
	}
	g_Notify->BreakPoint(__FILE__, __LINE__);
	return false;
}

bool NCA::GetTitleKey(CSwitchKeys & Keys, CSwitchKeys::KeyData & Key)
{
	if (!Keys.GetTitleKey(m_Header.rights_id, sizeof(m_Header.rights_id), Key))
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}
	CSwitchKeys::KeyData TitlekekKey;
	if (!Keys.GetKeyIndex(CSwitchKeys::TitlekekKey, GetCryptoRevision(), TitlekekKey))
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}

	CAESCipher cipher(TitlekekKey.data(), (uint32_t)TitlekekKey.size(), CAESCipher::CIPHER_AES_128_ECB);
	if (!cipher.Transcode(Key.data(), Key.data(), (uint32_t)Key.size()))
	{
		return false;
	}
	return true;
}

uint8_t NCA::GetCryptoRevision() const
{
	uint8_t master_key_id = m_Header.crypto_type_2 > m_Header.crypto_type ? m_Header.crypto_type_2 : m_Header.crypto_type;
	return master_key_id > 0 ? master_key_id - 1 : master_key_id;
}

bool NCA::IsDirectoryExeFS(CPartitionFilesystem* dir)
{
	if (dir == NULL)
	{
		return false;
	}
	if (dir->GetFile("main") == NULL)
	{
		return false;
	}
	if (dir->GetFile("main.npdm") == NULL)
	{
		return false;
	}
	return true;
}

const char * NCA::NCASectionFilesystemTypeName(NCASectionFilesystemType type)
{
	switch (type)
	{
	case NCASectionFilesystemType::PFS0: return "PFS0";
	case NCASectionFilesystemType::ROMFS: return "ROMFS";
	}
	return "unknown";
}
