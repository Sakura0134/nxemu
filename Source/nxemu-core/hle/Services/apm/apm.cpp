#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\apm\apm.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr APM::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new APM(System));
}

APM::APM(CSwitchSystem & System) :
    CService(System)
{
}

bool APM::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode APM::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
