#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\SystemGlobals.h>

CPUExecutor::CPUExecutor() :
    m_Reg(this)
{
}

void CPUExecutor::Execute(bool & Done)
{
	g_Notify->BreakPoint(__FILE__, __LINE__);
}