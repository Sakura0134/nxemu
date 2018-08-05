#include <nxemu-core\hle\HleKernel.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\StdString.h>

CHleKernel::CHleKernel(CSwitchSystem & System, CProcessMemory & ProcessMemory) :
    m_System(System),
    m_ProcessMemory(ProcessMemory),
    m_NextHandle(0x8002)
{
}

CHleKernel::~CHleKernel()
{
    for (size_t i = 0, n = m_SystemThreads.size(); i < n; i++)
    {
        delete m_SystemThreads[(int)i];
    }
    m_SystemThreads.clear();
}

bool CHleKernel::AddSystemThread(const char * name, uint64_t entry_point)
{
    uint32_t ThreadId = m_NextHandle++;
    CSystemThread * thread = new CSystemThread(this, m_ProcessMemory, name, entry_point, ThreadId);
    if (thread == NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    m_SystemThreads.insert(SystemThreadList::value_type(ThreadId, thread));
    return true;
}

