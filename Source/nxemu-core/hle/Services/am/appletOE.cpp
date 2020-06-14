#include <nxemu-core\hle\Services\am\appletOE.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr appletOE::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new appletOE(System));
}

appletOE::appletOE(CSwitchSystem & System) :
    CService(System)
{
}

bool appletOE::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode appletOE::CallMethod(CIPCRequest & Request)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
