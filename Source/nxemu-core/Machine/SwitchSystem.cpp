#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\Machine\PageTable.h>
#include <nxemu-core\FileFormat\formats.h>
#include <nxemu-core\FileFormat\xci.h>
#include <nxemu-core\FileFormat\nca.h>
#include <nxemu-core\FileFormat\nacp.h>
#include <nxemu-core\FileFormat\ProgramMetadata.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <nxemu-core\Settings\Settings.h>
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
	return true;
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
	const NCA * Program = m_Xci->Program();
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
	uint64_t exefs_offset = exefs->StartAddress() + exefs->Offset();
	CProgramMetadata metadata(exefs_offset, exefs->EncryptedFile(), npdm);

	uint64_t end_addr, base_addr = CProcessMemory::GetAddressSpaceBaseAddr();
	if (!LoadNSOModule(exefs_offset, exefs->EncryptedFile(), exefs->GetFile("rtld"), base_addr, end_addr)) { return false; }
	if (!LoadNSOModule(exefs_offset, exefs->EncryptedFile(), exefs->GetFile("main"), CPageTable::PageRoundUp(end_addr), end_addr)) { return false; }
	const CPartitionFilesystem::VirtualFiles Files = exefs->GetFiles();
	for (size_t i = 0; i < Files.size(); i++)
	{
		if (_strnicmp("subsdk", Files[i].Name.c_str(), 6) != 0)
		{
			continue;
		}
		if (!LoadNSOModule(exefs_offset, exefs->EncryptedFile(), exefs->GetFile(Files[i].Name.c_str()), CPageTable::PageRoundUp(end_addr), end_addr)) { return false; }
	}
	if (!LoadNSOModule(exefs_offset, exefs->EncryptedFile(), exefs->GetFile("sdk"), CPageTable::PageRoundUp(end_addr), end_addr)) { return false; }
	g_Notify->DisplayMessage(0, GS(MSG_LOADED_XCI));

    const NACP * Nacp = m_Xci->Nacp();
    if (Nacp == NULL)
    {
        return false;
    }
    g_Settings->SaveString(Game_File, XciFile);
    g_Settings->SaveString(Game_Name, Nacp->GetApplicationName().c_str());
    
	uint32_t ThreadId;
	if (!m_Kernel.AddSystemThread(ThreadId, "main", base_addr, 0, 0, metadata.GetMainThreadPriority(), 0))
	{
		return false;
	}
    WriteTrace(TraceGameFile, TraceInfo, "Done (res: true)");
	return true;
}

