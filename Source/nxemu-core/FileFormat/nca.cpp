#include <nxemu-core\FileFormat\nca.h>
#include <nxemu-core\Util\AESCipher.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>

NCA::NCA() :
	m_Header({ 0 }),
	m_Encrypted(false),
	m_HasRightsId(false)
{
}

NCA::~NCA()
{
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
			return false;
		}
		else if (section.raw.header.filesystem_type == NCASectionFilesystemType::PFS0)
		{
			return false;
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

const char * NCA::NCASectionFilesystemTypeName(NCASectionFilesystemType type)
{
	switch (type)
	{
	case NCASectionFilesystemType::PFS0: return "PFS0";
	case NCASectionFilesystemType::ROMFS: return "ROMFS";
	}
	return "unknown";
}
