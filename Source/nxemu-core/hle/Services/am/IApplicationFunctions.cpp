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

ResultCode IApplicationFunctions::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}