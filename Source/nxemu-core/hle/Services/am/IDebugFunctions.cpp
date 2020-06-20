#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\am\IDebugFunctions.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IDebugFunctions::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IDebugFunctions(System));
}

IDebugFunctions::IDebugFunctions(CSwitchSystem & System) :
    CService(System)
{
}

bool IDebugFunctions::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IDebugFunctions::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
