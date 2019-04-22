#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\SystemGlobals.h>

CPUExecutor::CPUExecutor(MemoryManagement & mmu) :
    m_Reg(this),
    m_MMU(mmu)
{
}

void CPUExecutor::Execute(bool & Done)
{
    /*uint64_t & PROGRAM_COUNTER = m_Reg.m_PROGRAM_COUNTER;

    while (!Done)
    {
		g_Notify->BreakPoint(__FILE__, __LINE__);
        PROGRAM_COUNTER += 4;    
    }*/
}