#include <nxemu-core\FileFormat\cnmt.h>

CNMT::CNMT(CEncryptedFile & EncryptedFile, int64_t BaseOffset, const CPartitionFilesystem::VirtualFile & File)
{
	size_t Offset = File.Offset;
	if (EncryptedFile.Seek(BaseOffset + Offset, CFileBase::begin) < 0)
	{
		return;
	}
	if (EncryptedFile.Read(&m_Header, sizeof(m_Header), Offset) != sizeof(m_Header))
	{
		return;
	}
	Offset += sizeof(m_Header);
	if (m_Header.type >= TitleType::Application && m_Header.type <= TitleType::AOC)
	{
		if (EncryptedFile.Read(&m_OptHeader, sizeof(m_OptHeader), Offset) != sizeof(m_OptHeader))
		{
			return;
		}
	}
	Offset += m_Header.table_offset;
	if (EncryptedFile.Seek(BaseOffset + Offset, CFileBase::begin) < 0)
	{
		return;
	}

	for (uint16_t i = 0; i < m_Header.number_content_entries; i++)
	{
		ContentRecord record = { 0 };
		if (EncryptedFile.Read(&record, sizeof(record), Offset) != sizeof(record))
		{
			return;
		}
		m_ContentRecords.push_back(record);
		Offset += sizeof(record);
	}
}
