#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\vi\IApplicationRootService.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IApplicationRootService::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IApplicationRootService(System));
}

IApplicationRootService::IApplicationRootService(CSwitchSystem & System) :
    CService(System)
{
}

bool IApplicationRootService::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IApplicationRootService::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
