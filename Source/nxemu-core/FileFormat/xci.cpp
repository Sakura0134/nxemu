#include <nxemu-core\FileFormat\xci.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\FileFormat\nsp.h>
#include <nxemu-core\Trace.h>
#include <set>

CXci::CXci(CSwitchKeys & Keys, const CPath & XciFile) :
	m_Valid(false),
	m_Header({0}),
	m_SecurePartition(NULL),
	m_Partitions(NULL)
{
	WriteTrace(TraceGameFile, TraceInfo, "Start (XciFile: \"%s\")", (const char *)XciFile);
	if (!XciFile.Exists())
	{
		WriteTrace(TraceGameFile, TraceError, "XciFile (\"%s\") - does not exist", (const char *)XciFile);
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}
	if (_stricmp(XciFile.GetExtension().c_str(), "xci") != 0)
	{
		WriteTrace(TraceGameFile, TraceError, "wrong extension (%s)", XciFile.GetExtension().c_str());
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}

	m_ReadFile.Open(XciFile, CFileBase::modeRead);
	if (!m_ReadFile.IsOpen())
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to open \"%s\"", (const char *)XciFile);
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}

	WriteTrace(TraceGameFile, TraceVerbose, "Reading Header");
	m_ReadFile.SeekToBegin();
	if (!m_ReadFile.Read(&m_Header, sizeof(m_Header)))
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to read header");
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}
	WriteTrace(TraceGameFile, TraceVerbose, "m_Header.Magic = \"%c%c%c%c\"", m_Header.Magic[0], m_Header.Magic[1], m_Header.Magic[2], m_Header.Magic[3]);
	if (*((uint32_t *)(&m_Header.Magic[0])) != *((uint32_t *)(&"HEAD")))
	{
		WriteTrace(TraceGameFile, TraceError, "Header magic is wrong");
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}

	WriteTrace(TraceGameFile, TraceVerbose, "HFS0PartitionOffset: 0x%I64u HFS0HeaderSize: 0x%I64u", m_Header.HFS0PartitionOffset, m_Header.HFS0HeaderSize);
	m_Partitions = new CPartitionFilesystem(CEncryptedFile(m_ReadFile), 0, m_Header.HFS0PartitionOffset, m_Header.HFS0HeaderSize);
	if (!m_Partitions->Valid())
	{
		WriteTrace(TraceGameFile, TraceError, "Partition Filesystem is not valid");
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}

	const CPartitionFilesystem::VirtualFile * SecureFile = m_Partitions->GetFile("secure");
	if (SecureFile == NULL)
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to find secure partitions");
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}
	
	WriteTrace(TraceGameFile, TraceVerbose, "Loading Secure partition");
	m_SecurePartition = new NSP(Keys, m_ReadFile, m_Header.HFS0PartitionOffset, SecureFile);
	if (!m_SecurePartition->Valid())
	{
		WriteTrace(TraceGameFile, TraceError, "Loaded secure partitions in not valid");
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}
	WriteTrace(TraceGameFile, TraceInfo, "Done");
}

CXci::~CXci()
{
	if (m_SecurePartition != NULL)
	{
		delete m_SecurePartition;
		m_SecurePartition = NULL;
	}
	if (m_Partitions != NULL)
	{
		delete m_Partitions;
		m_Partitions = NULL;
	}
}

bool CXci::IsXciFile(const CPath & File)
{
	if (!File.Exists() || _stricmp(File.GetExtension().c_str(), "xci") != 0)
	{
		return false;
	}

	CFile ReadFile(File, CFileBase::modeRead);
	if (!ReadFile.IsOpen())
	{
		return false;
	}

	GamecardHeader header;
	ReadFile.SeekToBegin();
	if (!ReadFile.Read(&header, sizeof(header)))
	{
		return false;
	}
	if (*((uint32_t *)(&header.Magic[0])) != *((uint32_t *)(&"HEAD")))
	{
		return false;
	}
	return true;
}