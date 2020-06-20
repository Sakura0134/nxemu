#include <nxemu-core\hle\Services\am\IDisplayController.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IDisplayController::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IDisplayController(System));
}

IDisplayController::IDisplayController(CSwitchSystem & System) :
    CService(System)
{
}

bool IDisplayController::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IDisplayController::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}