#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\path.h>
#include <Common\FileClass.h>

CSwitchSystem::CSwitchSystem() :
    m_EndEmulation(false),
    m_EmulationThread(stEmulationThread)
{
}

CSwitchSystem::~CSwitchSystem() 
{
}

bool CSwitchSystem::Initialize(void)
{
    return true;
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
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
}

bool CSwitchSystem::LoadGameDir(const char * GameDir)
{
    uint64_t end_addr, base_addr = CProcessMemory::GetAddressSpaceBaseAddr();
    if (!LoadNsoFile(CPath(GameDir, "rtld"), base_addr, end_addr)) { return false; }
    end_addr += 0x1000; //Set args?
    if (!LoadNsoFile(CPath(GameDir, "main"), (end_addr + 0xFFF) & ~0xFFF, end_addr)) { return false; }
    if (!LoadNsoFile(CPath(GameDir, "sdk"), (end_addr + 0xFFF) & ~0xFFF, end_addr)) { return false; }
    CPath SubSdk(GameDir, "subsdk*");
    for (bool FoundFile = SubSdk.FindFirst(); FoundFile; FoundFile = SubSdk.FindNext())
    {
        if (!LoadNsoFile(SubSdk, (end_addr + 0xFFF) & ~0xFFF, end_addr)) { return false; }
    }
    return true;
}
