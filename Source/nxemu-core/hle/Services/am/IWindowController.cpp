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
    switch (Request.RequestHeader().Command)
    {
    case GetAppletResourceUserId:
        ProcessGetAppletResourceUserId(Request);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void IWindowController::ProcessGetAppletResourceUserId(CIPCRequest & Request)
{
	CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
	ResponseData.resize(sizeof(uint64_t));
	*((uint64_t *)ResponseData.data()) = 0x51;
}
