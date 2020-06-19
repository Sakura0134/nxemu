#include <nxemu-core\hle\Services\am\IApplicationProxy.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IApplicationProxy::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IApplicationProxy(System));
}

IApplicationProxy::IApplicationProxy(CSwitchSystem & System) :
    CService(System)
{
}

bool IApplicationProxy::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IApplicationProxy::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}
