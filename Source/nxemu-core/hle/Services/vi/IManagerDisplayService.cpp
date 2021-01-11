#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\vi\IManagerDisplayService.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IManagerDisplayService::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IManagerDisplayService(System));
}

IManagerDisplayService::IManagerDisplayService(CSwitchSystem & System) :
    CService(System)
{
}

bool IManagerDisplayService::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IManagerDisplayService::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
