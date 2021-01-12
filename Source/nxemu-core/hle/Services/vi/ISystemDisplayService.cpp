#include <nxemu-core\hle\Services\vi\ISystemDisplayService.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr ISystemDisplayService::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new ISystemDisplayService(System));
}

ISystemDisplayService::ISystemDisplayService(CSwitchSystem & System) :
    CService(System)
{
}

bool ISystemDisplayService::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode ISystemDisplayService::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
