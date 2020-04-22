#include <nxemu-core\hle\HleKernel.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\StdString.h>

CHleKernel::CHleKernel(CSwitchSystem & System, CProcessMemory & ProcessMemory) :
    m_System(System),
    m_ProcessMemory(ProcessMemory),
    m_NextHandle(0x8002),
	m_NextThreadId(1)
{
}

CHleKernel::~CHleKernel()
{
    for (SystemThreadList::iterator itr = m_SystemThreads.begin(); itr != m_SystemThreads.end(); itr++)
    {
        delete itr->second;
    }
    m_SystemThreads.clear();
}

bool CHleKernel::AddSystemThread(uint32_t & ThreadHandle, const char * name, uint64_t entry_point, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId)
{
	ThreadHandle = GetNewHandle();
	CSystemThread * Thread = new CSystemThread(this, m_ProcessMemory, name, entry_point, ThreadHandle, CreateNewThreadID(), ThreadContext, StackTop, StackSize, Priority, ProcessorId);
    if (Thread == NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    Thread->Reg().Set64(Arm64Opcode::ARM64_REG_X1, ThreadHandle);
	m_SystemThreads.insert(SystemThreadList::value_type(ThreadHandle, Thread));
    return true;
}

uint32_t CHleKernel::GetNewHandle()
{
	uint32_t handle = m_NextHandle++;
	return handle;
}

uint64_t CHleKernel::CreateNewThreadID()
{
	uint64_t ThreadId = m_NextThreadId++;
	return ThreadId;
}

