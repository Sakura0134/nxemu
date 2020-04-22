#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\SystemGlobals.h>

CSystemThread::CSystemThread(CHleKernel * m_Kernel, CProcessMemory &ProcessMemory, const char * name, uint64_t entry_point, uint32_t ThreadHandle, uint32_t thread_id, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId) :
    CPUExecutor(m_ThreadMemory),
    m_Kernel(m_Kernel),
    m_ThreadMemory(ProcessMemory, this),
    m_thread_id(thread_id),
	m_Priority(Priority),
	m_Name(name != NULL ? name : "")
{
    if (!m_ThreadMemory.Initialize(StackTop, StackSize))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    m_Reg.Set64(Arm64Opcode::ARM64_REG_SP, StackTop);
    m_Reg.Set64(Arm64Opcode::ARM64_REG_PC, entry_point);

    for (size_t i = 0; i < 31; i++)
    {
        m_Reg.Set64((Arm64Opcode::arm64_reg)(Arm64Opcode::ARM64_REG_X0 + i), 0);
    }
    m_Reg.Set64(Arm64Opcode::ARM64_REG_X0, ThreadContext);
    m_Reg.SetConditionFlags(false, false, false, false);
}

CSystemThread::~CSystemThread()
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}