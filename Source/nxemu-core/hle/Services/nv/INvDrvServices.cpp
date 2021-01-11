#include <nxemu-core\hle\Services\nv\INvDrvServices.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr INvDrvServices::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new INvDrvServices(System));
}

INvDrvServices::INvDrvServices(CSwitchSystem & System) :
    CService(System)
{
}

INvDrvServices::~INvDrvServices()
{
}

bool INvDrvServices::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode INvDrvServices::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
