#include <nxemu-core\hle\Services\am\IWindowController.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IWindowController::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IWindowController(System));
}

IWindowController::IWindowController(CSwitchSystem & System) :
    CService(System)
{
}

bool IWindowController::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IWindowController::CallMethod(CIPCRequest & Request)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}