#include <nxemu-core\hle\Services\vi\IApplicationDisplayService.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IApplicationDisplayService::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IApplicationDisplayService(System));
}

IApplicationDisplayService::IApplicationDisplayService(CSwitchSystem & System) :
    CService(System)
{
}

bool IApplicationDisplayService::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IApplicationDisplayService::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}
