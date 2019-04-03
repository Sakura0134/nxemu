#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\FileFormat\formats.h>
#include <nxemu-core\FileFormat\xci.h>
#include <nxemu-core\FileFormat\nca.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\path.h>
#include <Common\FileClass.h>

CSwitchSystem::CSwitchSystem() :
    m_Kernel(*this,m_ProcessMemory),
    m_EndEmulation(false),
    m_EmulationThread(stEmulationThread),
	m_Xci(NULL)
{
}

CSwitchSystem::~CSwitchSystem() 
{
	if (m_Xci)
	{
		delete m_Xci;
	}
}

void CSwitchSystem::StartEmulation(void)
{
    m_EmulationThread.Start(this);
}

void CSwitchSystem::EmulationThread(void)
{
    bool & Done = m_EndEmulation;
    while (!Done)
    {
        CSystemThread* thread = m_Kernel.SystemThreads().begin()->second;
        thread->Execute(Done);
    }
}

bool CSwitchSystem::LoadGame(const char * GamePath)
{
	SwitchFileType type = IdentifyFileType(GamePath);

	if (type == sft_Unknown)
	{
		return false;
	}
	switch (type)
	{
	case sft_XCI:
		if (!LoadXCI(GamePath)) { return false; }
		break;
	default:
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}
	g_Notify->BreakPoint(__FILE__, __LINE__);
	return false;
}

bool CSwitchSystem::LoadXCI(const CPath & XciFile)
{
	WriteTrace(TraceGameFile, TraceInfo, "Start (XciFile: \"%s\")", (const char *)XciFile);
	std::auto_ptr<CXci> xci(new CXci(m_Keys, XciFile));
	if (xci.get() == NULL || !xci->Valid())
	{
		WriteTrace(TraceGameFile, TraceError, "xci is %s", xci.get() == NULL ? "NULL" : "Not Valid");
		WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
		return false;
	}
	m_Xci = xci.release();
	NCA * Program = m_Xci->Program();
	if (Program == NULL)
	{
		return false;
	}
	CPartitionFilesystem * exefs = Program->exefs();
	if (exefs == NULL)
	{
		return false;
	}
	const CPartitionFilesystem::VirtualFile * npdm = exefs->GetFile("main.npdm");
	if (npdm == NULL)
	{
		return false;
	}
	g_Notify->BreakPoint(__FILE__, __LINE__);
	WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
	return false;
}

