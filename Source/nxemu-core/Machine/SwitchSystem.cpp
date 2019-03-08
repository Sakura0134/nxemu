#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\path.h>
#include <Common\FileClass.h>

CSwitchSystem::CSwitchSystem() :
    m_Kernel(*this,m_ProcessMemory),
    m_EndEmulation(false),
    m_EmulationThread(stEmulationThread)
{
}

CSwitchSystem::~CSwitchSystem() 
{
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
	g_Notify->BreakPoint(__FILE__, __LINE__);
	return false;
}

