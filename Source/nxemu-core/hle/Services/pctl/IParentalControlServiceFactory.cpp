#include <nxemu-core\hle\Services\pctl\IParentalControlService.h>
#include <nxemu-core\hle\Services\pctl\IParentalControlServiceFactory.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IParentalControlServiceFactory::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IParentalControlServiceFactory(System));
}

IParentalControlServiceFactory::IParentalControlServiceFactory(CSwitchSystem & System) :
    CService(System)
{
}

bool IParentalControlServiceFactory::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IParentalControlServiceFactory::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case CreateService:
        Request.MakeObject(IParentalControlService::CreateInstance(m_System)->GetServicePtr());
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}
