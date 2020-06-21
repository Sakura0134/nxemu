#include <nxemu-core\hle\Services\pctl\IParentalControlService.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IParentalControlService::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IParentalControlService(System));
}

IParentalControlService::IParentalControlService(CSwitchSystem & System) :
    CService(System)
{
}

bool IParentalControlService::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IParentalControlService::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}