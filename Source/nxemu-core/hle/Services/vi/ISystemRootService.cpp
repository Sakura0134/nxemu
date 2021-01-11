#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\vi\ISystemRootService.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr ISystemRootService::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new ISystemRootService(System));
}

ISystemRootService::ISystemRootService(CSwitchSystem & System) :
    CService(System)
{
}

bool ISystemRootService::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode ISystemRootService::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
