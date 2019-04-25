#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Debugger.h>

CPUExecutor::CPUExecutor(MemoryManagement & mmu) :
    m_Reg(this),
    m_MMU(mmu)
{
}

void CPUExecutor::Execute(bool & Done)
{
    uint64_t & PROGRAM_COUNTER = m_Reg.m_PROGRAM_COUNTER;
    const bool & Stepping = isStepping();

    while (!Done)
    {
        g_Settings->SaveBool(Debugger_SteppingOps, true);
        if (Stepping)
        {
            g_Debugger->WaitForStep();
        }
		g_Notify->BreakPoint(__FILE__, __LINE__);
        PROGRAM_COUNTER += 4;    
    }
}