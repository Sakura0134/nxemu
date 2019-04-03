#include <nxemu-core\FileFormat\nca.h>
#include <nxemu-core\Trace.h>

NCA::NCA()
{
}

NCA::~NCA()
{
}

bool NCA::Load(CSwitchKeys & Keys, CFile & file, int64_t BaseOffset, int64_t FileOffset, uint64_t Size)
{
	WriteTrace(TraceGameFile, TraceVerbose, "Start (FileOffset: 0x%I64u Size: 0x%I64u)", FileOffset, Size);
	WriteTrace(TraceGameFile, TraceVerbose, "Done (res: false)");
	return false;
}
