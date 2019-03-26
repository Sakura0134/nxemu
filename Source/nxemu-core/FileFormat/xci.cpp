#include <nxemu-core\FileFormat\xci.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\Trace.h>
#include <set>

CXci::CXci(CSwitchKeys & Keys, const CPath & XciFile) :
	m_Valid(false),
	m_Header({0})
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
	if (m_Header.Magic != *((uint32_t *)(&"HEAD")))
	{
		WriteTrace(TraceGameFile, TraceError, "Header magic is wrong");
		WriteTrace(TraceGameFile, TraceInfo, "Done");
		return;
	}

	WriteTrace(TraceGameFile, TraceInfo, "Done");
}

CXci::~CXci()
{
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
	if (header.Magic != *((uint32_t *)(&"HEAD")))
	{
		return false;
	}
	return true;
}