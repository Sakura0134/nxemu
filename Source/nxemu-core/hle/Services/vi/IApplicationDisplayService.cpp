#include <nxemu-core\hle\Services\vi\IApplicationDisplayService.h>
#include <nxemu-core\hle\Services\vi\IManagerDisplayService.h>
#include <nxemu-core\hle\Services\vi\ISystemDisplayService.h>
#include <nxemu-core\hle\Services\vi\IHOSBinderDriver.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IApplicationDisplayService::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IApplicationDisplayService(System));
}

IApplicationDisplayService::IApplicationDisplayService(CSwitchSystem & System) :
    CService(System)
{
}

bool IApplicationDisplayService::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IApplicationDisplayService::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_GetRelayService:
		Request.MakeObject(IHOSBinderDriver::CreateInstance(m_System)->GetServicePtr());
	    break;
    case Method_GetSystemDisplayService:
        Request.MakeObject(ISystemDisplayService::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method_GetManagerDisplayService:
        Request.MakeObject(IManagerDisplayService::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method_OpenDisplay:
        return ViOpenDisplay(Request);
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return 0;
}

ResultCode IApplicationDisplayService::ViOpenDisplay(CIPCRequest & Request)
{
    const CIPCRequest::REQUEST_DATA & data = Request.RequestData();
    std::string name((const char *)data.data(), 8);

    CDisplay & Display = m_System.HleKernel().GetDisplay();
    uint32_t DisplayId = 0;
    if (!Display.Open(name.c_str(), DisplayId))
    {
        return VI_ERR_NOT_FOUND;
    }

    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint64_t));
	((uint64_t *)ResponseData.data())[0] = DisplayId;
    return RESULT_SUCCESS;
}