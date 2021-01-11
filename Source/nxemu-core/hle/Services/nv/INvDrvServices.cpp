#include <nxemu-core\hle\Services\nv\INvDrvServices.h>
#include <nxemu-core\hle\Display\Nvidia\nvResult.h>
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

ResultCode INvDrvServices::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_Initialize:
        NvInitialize(Request);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}

void INvDrvServices::NvInitialize(CIPCRequest & Request)
{
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t));
    *((uint32_t *)ResponseData.data()) = nvResult_Success;
}
