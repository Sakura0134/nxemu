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
        CKernelObjectPtr ThreadObject(NULL);
        {
            KernelObjectList & ThreadQueue = m_Kernel.ThreadQueue();
            for (size_t i = 0, n = ThreadQueue.size(); i < n; i++)
            {
                ThreadObject = ThreadQueue[i];
                break;
            }
        }

        if (ThreadObject.get() == NULL || ThreadObject->GetHandleType() != CKernelObject::HandleType::Thread)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }
        m_SystemThread.Set(ThreadObject.get());
        CSystemThread * Thread = ThreadObject->GetSystemThreadPtr();
        Thread->Execute(Done);
        KernelObjectList & ThreadQueue = m_Kernel.ThreadQueue();
        for (size_t i = 0, n = ThreadQueue.size(); i < n; i++)
        {
            if (ThreadQueue[i] != Thread)
            {
                continue;
            }
            ThreadQueue.erase(ThreadQueue.begin() + i);
            ThreadQueue.push_back(Thread);
            break;
        }
        m_SystemThread.Set(NULL);
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
        WriteTrace(TraceGameFile, TraceError, "Failed to find Program");
        WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
        return false;
    }
    CPartitionFilesystem * exefs = Program->exefs();
    if (exefs == NULL)
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to find exefs");
        WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
        return false;
    }
    const NACP * Nacp = m_Xci->Nacp();
    if (Nacp == NULL)
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to get Nacp");
        WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
        return false;
    }
    const CProgramMetadata * Metadata = m_Xci->Metadata();
    if (Metadata == NULL)
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to get Metadata");
        WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
        return false;
    }

    if (!Metadata->Is64bit())
    {
        WriteTrace(TraceGameFile, TraceError, "XCI is not 64bit");
        WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
        return false;
    }
    if (!m_ProcessMemory.Initialize(Metadata->GetAddressSpaceType(), Metadata->Is64bit()))
    {
        WriteTrace(TraceGameFile, TraceError, "failed to Initialize process memory");
        WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
        return false;
    }

    uint64_t exefs_offset = exefs->StartAddress() + exefs->Offset();
    uint64_t end_addr, base_addr = m_ProcessMemory.GetAddressSpaceBaseAddr();
    if (!LoadNSOModule(exefs_offset, exefs->EncryptedFile(), exefs->GetFile("rtld"), base_addr, end_addr))
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to load rtld");
        WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
        return false;
    }
    if (!LoadNSOModule(exefs_offset, exefs->EncryptedFile(), exefs->GetFile("main"), CPageTable::PageRoundUp(end_addr), end_addr))
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to load main");
        WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
        return false;
    }
    const CPartitionFilesystem::VirtualFiles Files = exefs->GetFiles();
    for (size_t i = 0; i < Files.size(); i++)
    {
        if (_strnicmp("subsdk", Files[i].Name.c_str(), 6) != 0)
        {
            continue;
        }
        if (!LoadNSOModule(exefs_offset, exefs->EncryptedFile(), exefs->GetFile(Files[i].Name.c_str()), CPageTable::PageRoundUp(end_addr), end_addr))
        {
            WriteTrace(TraceGameFile, TraceError, "Failed to load %s", Files[i].Name.c_str());
            WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
            return false;
        }
    }
    if (!LoadNSOModule(exefs_offset, exefs->EncryptedFile(), exefs->GetFile("sdk"), CPageTable::PageRoundUp(end_addr), end_addr))
    {
        WriteTrace(TraceGameFile, TraceError, "Failed to load sdk");
        WriteTrace(TraceGameFile, TraceInfo, "Done (res: false)");
        return false;
    }

    g_Notify->DisplayMessage(0, GS(MSG_LOADED_XCI));
    g_Settings->SaveString(Game_File, XciFile);
    g_Settings->SaveString(Game_Name, Nacp->GetApplicationName().c_str());

    uint32_t ThreadHandle;
    uint64_t StackAreaVaddrEnd = m_ProcessMemory.GetTlsIoRegionBase() + m_ProcessMemory.GetTlsIoRegionSize();
    if (!m_Kernel.AddSystemThread(ThreadHandle, "main", base_addr, 0, StackAreaVaddrEnd, Metadata->GetMainThreadStackSize(), Metadata->GetMainThreadPriority(), 0))
    {
        return false;
    }
    WriteTrace(TraceGameFile, TraceInfo, "Done (res: true)");
    return true;
}
