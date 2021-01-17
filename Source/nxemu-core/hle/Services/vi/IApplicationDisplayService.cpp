#include <nxemu-core\hle\Services\vi\IApplicationDisplayService.h>
#include <nxemu-core\hle\Services\vi\IManagerDisplayService.h>
#include <nxemu-core\hle\Services\vi\ISystemDisplayService.h>
#include <nxemu-core\hle\Services\vi\IHOSBinderDriver.h>
#include <nxemu-core\hle\Services\vi\ViParcel.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

class CNativeWindow : 
    public ViParcel {
public:
    CNativeWindow(uint32_t id) :
        m_data({ 0 })
    {
        m_data.magic = 2;
        m_data.process_id = 1;
        m_data.id = id;
        strcpy((char *)&m_data.dispdrv[0], "dispdrv");
    }

protected:
    void SerializeData() 
    {
        Write(&m_data,sizeof(m_data));
    }

private:
    struct Data 
    {
        uint32_t magic;
        uint32_t process_id;
        uint32_t id;
        uint32_t padding[3];
        uint8_t dispdrv[8];
        uint32_t padding2[2];
    };

    Data m_data;
};

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
    case Method_OpenLayer:
        return ViOpenLayer(Request);
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

ResultCode IApplicationDisplayService::ViOpenLayer(CIPCRequest & Request)
{
    const CIPCRequest::REQUEST_DATA & data = Request.RequestData();
    std::string DisplayName((const char *)data.data(), 0x40);

    CDisplay & Display = m_System.HleKernel().GetDisplay();
    uint32_t DisplayId = 0;
    if (!Display.Open(DisplayName.c_str(), DisplayId))
    {
        return VI_ERR_NOT_FOUND;
    }
    uint64_t LayerId = *((uint64_t *)&data[0x40]);

    uint32_t BufferQueueId;
    if (!Display.FindBufferQueueId(DisplayId, (uint32_t)LayerId, BufferQueueId))
    {
        return VI_ERR_NOT_FOUND;
    }

    CNativeWindow NativeWindow(BufferQueueId);
    CIPCRequest::RequestBuffer NativeWindowData = NativeWindow.Serialize();
    Request.WriteBuffer(NativeWindowData);

    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint64_t));
    ((uint64_t *)ResponseData.data())[0] = NativeWindowData.size();
    return RESULT_SUCCESS;
}

