#include <nxemu-core\hle\SystemThread.h>
#include <nxemu-core\SystemGlobals.h>

CSystemThread::CSystemThread(CHleKernel * m_Kernel, CProcessMemory &ProcessMemory, const char * name, uint64_t entry_point, uint32_t ThreadHandle, uint32_t thread_id, uint64_t ThreadContext, uint64_t StackTop, uint32_t Priority, uint32_t ProcessorId) :
    m_Kernel(m_Kernel),
    m_thread_id(thread_id),
	m_Priority(Priority),
	m_Name(name != NULL ? name : "")
{
    m_Reg.Set64(Arm64Opcode::ARM64_REG_PC, entry_point);
}

CSystemThread::~CSystemThread()
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}