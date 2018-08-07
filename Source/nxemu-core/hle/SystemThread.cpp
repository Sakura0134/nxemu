#include <nxemu-core\hle\SystemThread.h>
#include <nxemu-core\SystemGlobals.h>

CSystemThread::CSystemThread(CHleKernel * m_Kernel, CProcessMemory &ProcessMemory, const char * /*name*/, uint64_t entry_point, uint32_t thread_id) :
    m_Kernel(m_Kernel)
{
    m_Reg.Set64(Arm64Opcode::ARM64_REG_PC, entry_point);
}

CSystemThread::~CSystemThread()
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}