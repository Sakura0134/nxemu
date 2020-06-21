#include <nxemu-core\hle\Services\ns\IAddOnContentManager.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IAddOnContentManager::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IAddOnContentManager(System));
}

IAddOnContentManager::IAddOnContentManager(CSwitchSystem & System) :
    CService(System)
{
}

bool IAddOnContentManager::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IAddOnContentManager::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
