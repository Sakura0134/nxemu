#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CServiceManger::CServiceManger(CSwitchSystem & System) :
    CService(System)
{
}

bool CServiceManger::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode CServiceManger::CallMethod(CIPCRequest & /*Request*/)
{    
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}