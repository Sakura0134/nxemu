#include <nxemu-core\FileFormat\xci.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\FileFormat\nsp.h>
#include <nxemu-core\FileFormat\nca.h>
#include <nxemu-core\FileFormat\nacp.h>
#include <nxemu-core\FileFormat\romfs.h>
#include <nxemu-core\FileFormat\ProgramMetadata.h>
#include <nxemu-core\Trace.h>
#include <set>

CXci::CXci(CSwitchKeys & Keys, const CPath & XciFile) :
	m_Valid(false),
	m_Header({0}),
	m_SecurePartition(nullptr),
	m_Partitions(nullptr),
    m_Program(nullptr),
    m_Control(nullptr),
    m_Nacp(nullptr),
    m_MetaData(nullptr)
{
	WriteTrace(TraceGameFile, TraceDebug, "Start (XciFile: \"%s\")", (const char *)XciFile);
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
	if (SecureFile == nullptr)
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
	uint64_t ProgramTitleID = m_SecurePartition->GetProgramTitleID();
	WriteTrace(TraceGameFile, TraceVerbose, "ProgramTitleID: 0x%I64u", ProgramTitleID);
	m_Program = m_SecurePartition->GetNCA(ProgramTitleID, CNMT::ContentRecordType::Program);
	if (m_Program == nullptr)
	{
		WriteTrace(TraceGameFile, TraceError, "Failed to find Get NCA (ProgramTitleID: 0x%I64u, type: ContentRecordType::Program)", ProgramTitleID);
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}
    m_Control = m_SecurePartition->GetNCA(ProgramTitleID, CNMT::ContentRecordType::Control);
    if (m_Control == nullptr)
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to find Get NCA (ProgramTitleID: 0x%I64u, type: ContentRecordType::Control)", ProgramTitleID);
        WriteTrace(TraceGameFile, TraceInfo, "Done");
        return;
    }
    if (!ParseControlNCA())
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to Parse Control NCA");
        WriteTrace(TraceGameFile, TraceInfo, "Done");
        return;
    }

    CPartitionFilesystem * exefs = m_Program->exefs();
    if (exefs == nullptr)
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to find exefs");
        WriteTrace(TraceGameFile, TraceInfo, "Done");
        return;
    }
    const CPartitionFilesystem::VirtualFile * npdm = exefs->GetFile("main.npdm");
    if (npdm == nullptr)
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to find main.npdm in exefs");
        WriteTrace(TraceGameFile, TraceInfo, "Done");
        return;
    }
    uint64_t exefs_offset = exefs->StartAddress() + exefs->Offset();
    m_MetaData = new CProgramMetadata(exefs_offset, exefs->EncryptedFile(), npdm);

    WriteTrace(TraceGameFile, TraceVerbose, "xci is valid");
	m_Valid = true;
	WriteTrace(TraceGameFile, TraceDebug, "Done");
}

CXci::~CXci()
{
	if (m_SecurePartition != nullptr)
	{
		delete m_SecurePartition;
		m_SecurePartition = nullptr;
	}
	if (m_Partitions != nullptr)
	{
		delete m_Partitions;
		m_Partitions = nullptr;
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

bool CXci::ParseControlNCA(void)
{
    if (m_Control == nullptr)
    {
        return false;
    }
    RomFS fs;
    if (!fs.Load(m_Control->Romfs(), m_Control->RomfsOffset()))
    {
        return false;
    }
    const RomFS::FILE * nacp_file = fs.GetFile("control.nacp");
    if (nacp_file == nullptr)
    {
        return false;
    }
    m_Nacp = new NACP;
    if (!m_Nacp->Load(m_Control->Romfs(), m_Control->RomfsOffset(), nacp_file->offset, nacp_file->size))
    {
        return false;
    }
    return true;
}
