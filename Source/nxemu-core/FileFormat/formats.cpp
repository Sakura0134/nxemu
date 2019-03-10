#include <nxemu-core\FileFormat\formats.h>
#include <nxemu-core\FileFormat\xci.h>
#include <Common\path.h>

bool IsNroFile(const CPath & File);

SwitchFileType IdentifyFileType(const char * File)
{
	CPath path(File);
	if (CXci::IsXciFile(path)) { return sft_XCI; }
	return sft_Unknown;
}
