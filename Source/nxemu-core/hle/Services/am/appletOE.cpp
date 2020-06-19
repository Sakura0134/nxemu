#include <nxemu-core\hle\Services\am\appletOE.h>
#include <nxemu-core\hle\Services\am\IApplicationProxy.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr appletOE::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new appletOE(System));
}

appletOE::appletOE(CSwitchSystem & System) :
    CService(System)
{
}

bool appletOE::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode appletOE::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case OpenApplicationProxy:
        Request.MakeObject(IApplicationProxy::CreateInstance(m_System)->GetServicePtr());
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}
