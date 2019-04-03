#include <nxemu-core\FileFormat\nsp.h>
#include <nxemu-core\FileFormat\nca.h>
#include <nxemu-core\Trace.h>

NSP::NSP(CSwitchKeys & Keys, CFile & ReadFile, int64_t PartitionOffset, const CPartitionFilesystem::VirtualFile * file) :
	m_Valid(false),
	m_Files(NULL)
{
	WriteTrace(TraceGameFile, TraceVerbose, "Start (PartitionOffset: 0x%I64u file->Offset: 0x%I64u file->Size: 0x%I64u)", PartitionOffset, file->Offset, file->Size);
	m_Files = new CPartitionFilesystem(CEncryptedFile(ReadFile), PartitionOffset, file->Offset, file->Size);
	if (!m_Files->Valid())
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to load Partition Filesystem");
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}

	if (!ReadTicketKeys(Keys, ReadFile, PartitionOffset, file)) { return;  }
	if (!ReadNCAs(Keys, ReadFile, PartitionOffset, file)) { return; }
	m_Valid = true;
    WriteTrace(TraceGameFile, TraceInfo, "Done");
}

NSP::~NSP()
{
    WriteTrace(TraceGameFile, TraceInfo, "Start");

	for (size_t i = 0; i < m_Ncas.size(); i++)
	{
		delete m_Ncas[i];
	}
	m_Ncas.clear();

	if (m_Files != NULL)
	{
		delete m_Files;
		m_Files = NULL;
	}
    WriteTrace(TraceGameFile, TraceInfo, "Done");
}

bool NSP::ReadTicketKeys(CSwitchKeys & Keys, CFile & ReadFile, int64_t PartitionOffset, const CPartitionFilesystem::VirtualFile * file)
{
	WriteTrace(TraceGameFile, TraceVerbose, "Start (PartitionOffset: 0x%I64u)", PartitionOffset);
	enum { TICKET_FILE_TITLEKEY_OFFSET = 0x180 };

	const VirtualFiles & Files = m_Files->GetFiles();
	for (VirtualFiles::const_iterator itr = Files.begin(); itr != Files.end(); itr++)
	{
		size_t NameLen = itr->Name.length();
		if (NameLen < 4 || strcmp(&itr->Name[NameLen - 4], ".tik") != 0)
		{
			WriteTrace(TraceGameFile, TraceVerbose, "ignore: Not a ticket file (Name: %s)", itr->Name.c_str());
			continue;
		}

		WriteTrace(TraceGameFile, TraceVerbose, "Found ticket file (Name: %s)", itr->Name.c_str());
		if (!ReadFile.IsOpen() || ReadFile.Seek(PartitionOffset + file->Offset + itr->Offset + TICKET_FILE_TITLEKEY_OFFSET, CFileBase::begin) < 0)
		{
			WriteTrace(TraceGameFile, TraceError, "Failed to seek to ticket file");
			WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
			return false;
		}

		CSwitchKeys::KeyData Key(0x10);
		if (!ReadFile.Read(Key.data(), (uint32_t)Key.size()))
		{
			WriteTrace(TraceGameFile, TraceError, "Failed to read key");
			WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
			return false;
		}

		std::string Name(itr->Name);
		Name.erase(Name.size() - 4);

		std::vector<uint8_t> rights_id;
		for (size_t i = 0, n = (Name.size() >> 1); i < n; i++)
		{
			rights_id.push_back((ToHexNibble(Name[i << 1]) << 4) | ToHexNibble(Name[(i << 1) + 1]));
		}
		WriteTrace(TraceGameFile, TraceVerbose, "RightsID: %s key: %02X%02X%02X%02X%02X....", Name.c_str(), Key[0], Key[1], Key[2], Key[3], Key[4]);
		Keys.SetTitleKey(rights_id.data(), rights_id.size(), Key);
	}
	WriteTrace(TraceGameFile, TraceVerbose, "Done (res: True)");
	return true;
}

bool NSP::ReadNCAs(CSwitchKeys & Keys, CFile & ReadFile, int64_t PartitionOffset, const CPartitionFilesystem::VirtualFile * file)
{
	WriteTrace(TraceGameFile, TraceVerbose, "Start (Name: \"%s\" PartitionOffset: 0x%I64u )", file->Name.c_str(), PartitionOffset);

	const VirtualFiles & Files = m_Files->GetFiles();
	size_t FileOffset = PartitionOffset + file->Offset;
	for (VirtualFiles::const_iterator itr = Files.begin(); itr != Files.end(); itr++)
	{
		size_t NameLen = itr->Name.length();
		if (NameLen < 9 || strcmp(&itr->Name[NameLen - 9], ".cnmt.nca") != 0)
		{
			WriteTrace(TraceGameFile, TraceVerbose, "ignoring \"%s\"", itr->Name.c_str());
			continue;
		}
		WriteTrace(TraceGameFile, TraceVerbose, "Loading \"%s\"", itr->Name.c_str());
		NCA * nca = new NCA;
		if (!nca->Load(Keys, ReadFile, FileOffset, itr->Offset, itr->Size))
		{
			WriteTrace(TraceGameFile, TraceError, "Failed to load \"%s\"", itr->Name.c_str());
			delete nca;
			WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
			return false;
		}
		m_Ncas.push_back(nca);

		const NCA::CPartitionFilesystems Dirs = nca->Dirs();
		if (Dirs.size() < 1)
		{
			WriteTrace(TraceGameFile, TraceVerbose, "No directories in \"%s\"", itr->Name.c_str());
			continue;
		}

		const VirtualFiles & Files0 = Dirs[0]->GetFiles();
		CEncryptedFile & EncryptedFile = Dirs[0]->EncryptedFile();
		for (VirtualFiles::const_iterator itr0 = Files0.begin(); itr0 != Files0.end(); itr0++)
		{
			NameLen = itr0->Name.length();
			if (NameLen < 5 || strcmp(&itr0->Name[NameLen - 5], ".cnmt") != 0)
			{
				WriteTrace(TraceGameFile, TraceVerbose, "ignoring file \"%s\"", itr0->Name.c_str());
				continue;
			}
			return false;
		}
	}
	WriteTrace(TraceGameFile, TraceVerbose, "Done (res: True)");
	return true;
}


uint8_t NSP::ToHexNibble(char c)
{
	if (c >= 'A' && c <= 'F')
	{
		return (c - 'A') + 10;
	}
	if (c >= 'a' && c <= 'f')
	{
		return (c - 'a') + 10;
	}
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	return 0;
}
