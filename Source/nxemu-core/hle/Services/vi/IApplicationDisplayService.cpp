#include <nxemu-core\hle\Services\vi\IApplicationDisplayService.h>
#include <nxemu-core\hle\Services\vi\IManagerDisplayService.h>
#include <nxemu-core\hle\Services\vi\ISystemDisplayService.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IApplicationDisplayService::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IApplicationDisplayService(System));
}

IApplicationDisplayService::IApplicationDisplayService(CSwitchSystem & System) :
    CService(System)
{
}

bool IApplicationDisplayService::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IApplicationDisplayService::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_GetSystemDisplayService:
        Request.MakeObject(ISystemDisplayService::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method_GetManagerDisplayService:
        Request.MakeObject(IManagerDisplayService::CreateInstance(m_System)->GetServicePtr());
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return 0;
}
