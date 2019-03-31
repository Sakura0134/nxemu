#include <nxemu-core\FileSystem\PartitionFilesystem.h>
#include <nxemu-core\Trace.h>

CPartitionFilesystem::CPartitionFilesystem(CEncryptedFile EncryptedFile, size_t Offset, uint64_t StartAddress, uint64_t Size) :
	m_EncryptedFile(EncryptedFile),
	m_Offset(Offset),
	m_StartAddress(StartAddress),
	m_Size(Size),
	m_Valid(false)
{
	m_Valid = Load(Offset + StartAddress, Size);
}

bool CPartitionFilesystem::Load(int64_t Offset, uint64_t Size)
{
	WriteTrace(TraceGameFile, TraceVerbose, "Start (Offset: 0x%I64u Size: 0x%I64u)", Offset, Size);

	struct Header
	{
		uint8_t MagicNum[4];
		uint32_t NumberOfFiles;
		uint32_t SizeOfStringTable;
		uint32_t Reserved;
	};

	if (m_EncryptedFile.GetLength() < Offset + Size)
	{
		WriteTrace(TraceGameFile, TraceError, "Encrypted File to small (m_EncryptedFile.GetLength() = 0x%I64u Offset: 0x%I64u Size: 0x%I64u)", Offset, Size);
		return false;
	}

	if (Size < sizeof(Header))
	{
		WriteTrace(TraceGameFile, TraceError, "Encrypted File to small (sizeof(Header) = 0x%I64u Size: 0x%I64u)", (uint64_t)sizeof(Header), Size);
		return false;
	}
	Header fs_header;
	m_EncryptedFile.Seek(Offset, CFileBase::begin);
	if (m_EncryptedFile.Read(&fs_header, sizeof(fs_header), 0) != sizeof(fs_header))
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to read header (sizeof(Header) = 0x%I64u)", (uint64_t)sizeof(Header));
		return false;
	}
	WriteTrace(TraceGameFile, TraceVerbose, "fs_header.MagicNum = \"%c%c%c%c\"", fs_header.MagicNum[0], fs_header.MagicNum[1], fs_header.MagicNum[2], fs_header.MagicNum[3]);
	if (*((uint32_t *)(&fs_header.MagicNum[0])) != *((uint32_t *)(&"HFS0")) &&
		*((uint32_t *)(&fs_header.MagicNum[0])) != *((uint32_t *)(&"PFS0")))
	{
		WriteTrace(TraceGameFile, TraceError, "invaild header Magic Number (\"%c%c%c%c\")", fs_header.MagicNum[0], fs_header.MagicNum[1], fs_header.MagicNum[2], fs_header.MagicNum[3]);
		return false;
	}
	bool is_hfs = *((uint32_t *)(&fs_header.MagicNum[0])) == *((uint32_t *)(&"HFS0"));
	WriteTrace(TraceGameFile, TraceVerbose, "is hfs: %s", is_hfs ? "yes" : "no");

	struct FileEntry
	{
		uint64_t Offset;
		uint64_t Size;
		uint32_t StringTableOffset;
		uint32_t HashedRegionSize;
		uint64_t Reserved;
		uint8_t Hash[0x20];
	};

	struct PFS0FileEntry 
	{
		uint64_t Offset;
		uint64_t Size;
		uint32_t StringTableOffset;
		uint32_t Reserved;
	};

	std::size_t EntrySize = is_hfs ? sizeof(FileEntry) : sizeof(PFS0FileEntry);
	WriteTrace(TraceGameFile, TraceVerbose, "EntrySize: 0x%X", EntrySize);
	std::size_t MetaDataSize = sizeof(Header) + (fs_header.NumberOfFiles * EntrySize) + fs_header.SizeOfStringTable;
	WriteTrace(TraceGameFile, TraceVerbose, "MetaDataSize: 0x%X", MetaDataSize);
	m_EncryptedFile.Seek(Offset, CFileBase::begin);
	std::vector<uint8_t> MetaData(MetaDataSize);
	if (m_EncryptedFile.Read(MetaData.data(), (uint32_t)MetaDataSize, 0) != MetaDataSize)
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to read meta data (MetaDataSize: 0x%0X)", MetaDataSize);
		return false;
	}

	size_t EntriesOffset = sizeof(Header);
	size_t StringTableOffset = EntriesOffset + (fs_header.NumberOfFiles * EntrySize);
	std::vector<uint8_t> StringTable(&MetaData[StringTableOffset], &MetaData[StringTableOffset] + fs_header.SizeOfStringTable);

	WriteTrace(TraceGameFile, TraceVerbose, "fs_header.NumberOfFiles: %d", fs_header.NumberOfFiles);
	for (uint32_t i = 0; i < fs_header.NumberOfFiles; i++)
	{
		FileEntry & entry = *((FileEntry *)&MetaData[EntriesOffset + (i * EntrySize)]);

		VirtualFile File;
		File.Name = std::string((const char*)&StringTable[entry.StringTableOffset]);
		File.Offset = MetaDataSize + entry.Offset;
		File.Size = entry.Size;
		WriteTrace(TraceGameFile, TraceVerbose, "%d: Name: \"%s\" Offset: 0x%I64u Size: 0x%I64u", i, File.Name.c_str(), File.Offset, File.Size);
		m_Files.push_back(File);
	}
	WriteTrace(TraceGameFile, TraceVerbose, "Done");
	return true;
}

