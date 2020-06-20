#include <nxemu-core\hle\Services\am\ICommonStateGetter.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr ICommonStateGetter::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new ICommonStateGetter(System));
}

ICommonStateGetter::ICommonStateGetter(CSwitchSystem & System) :
    CService(System)
{
}

bool ICommonStateGetter::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode ICommonStateGetter::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
