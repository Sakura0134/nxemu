#include <nxemu-core\hle\Services\hid\IAppletResource.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IAppletResource::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IAppletResource(System));
}

IAppletResource::IAppletResource(CSwitchSystem & System) :
    CService(System)
{
}

bool IAppletResource::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

void IAppletResource::Close(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

ResultCode IAppletResource::CallMethod(CIPCRequest & Request)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}