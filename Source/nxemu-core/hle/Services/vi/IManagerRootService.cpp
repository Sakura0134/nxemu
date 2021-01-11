#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\vi\IApplicationDisplayService.h>
#include <nxemu-core\hle\Services\vi\IManagerRootService.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IManagerRootService::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IManagerRootService(System));
}

IManagerRootService::IManagerRootService(CSwitchSystem & System) :
    CService(System)
{
}

bool IManagerRootService::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IManagerRootService::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_GetDisplayService:
		Request.MakeObject(IApplicationDisplayService::CreateInstance(m_System)->GetServicePtr());
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}
