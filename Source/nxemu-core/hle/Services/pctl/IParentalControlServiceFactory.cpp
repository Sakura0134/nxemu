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

ResultCode IParentalControlServiceFactory::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
