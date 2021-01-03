#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\ssl\SSLServices.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr SSLServices::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new SSLServices(System));
}

SSLServices::SSLServices(CSwitchSystem & System) :
    CService(System)
{
}

bool SSLServices::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode SSLServices::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
