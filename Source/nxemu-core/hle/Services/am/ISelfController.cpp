#include <nxemu-core\hle\Services\am\ISelfController.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr ISelfController::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new ISelfController(System));
}

ISelfController::ISelfController(CSwitchSystem & System) :
    CService(System)
{
}

bool ISelfController::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode ISelfController::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}