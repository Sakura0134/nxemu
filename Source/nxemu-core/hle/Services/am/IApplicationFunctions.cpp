#include <nxemu-core\hle\Services\am\IApplicationFunctions.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IApplicationFunctions::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IApplicationFunctions(System));
}

IApplicationFunctions::IApplicationFunctions(CSwitchSystem & System) :
    CService(System)
{
}

bool IApplicationFunctions::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IApplicationFunctions::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method::NotifyRunning: ProcessNotifyRunning(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}
void IApplicationFunctions::ProcessNotifyRunning(CIPCRequest & Request)
{
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t));
    ((uint32_t *)ResponseData.data())[0] = 0;
}

