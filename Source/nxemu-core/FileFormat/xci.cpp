#include <nxemu-core\FileFormat\xci.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\Trace.h>
#include <set>

CXci::CXci(CSwitchKeys & Keys, const CPath & XciFile) :
	m_Valid(false)
{
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