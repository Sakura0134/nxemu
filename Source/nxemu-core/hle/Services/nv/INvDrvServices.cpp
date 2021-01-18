#include <nxemu-core\hle\Services\nv\INvDrvServices.h>
#include <nxemu-core\hle\Display\Nvidia\nvResult.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr INvDrvServices::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new INvDrvServices(System));
}

INvDrvServices::INvDrvServices(CSwitchSystem & System) :
    CService(System),
    m_NvDriver(System.NvDriver())
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
    case Method_Open:
        NvOpen(Request);
        break;
    case Method_Ioctl:
        NvIoctl(Request);
        break;
    case Method_Initialize:
        NvInitialize(Request);
        break;
    case Method_QueryEvent:
        NvQueryEvent(Request);
        break;
	case Method_SetAruid:
		NvSetAruid(Request);
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

void INvDrvServices::NvIoctl(CIPCRequest & Request)
{
    if (Request.RequestData().size() < 8)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    uint32_t Fd = *(uint32_t *)(&Request.RequestData()[0]);
    nvIoctl Ioctl = *(nvIoctl*)(&Request.RequestData()[4]);

    CIPCRequest::RequestBuffer InData, OutData;
    if (Ioctl.In != 0 && !Request.ReadBuffer(InData))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    nvResult result = m_NvDriver.Ioctl(Fd, Ioctl, InData, OutData);
    CIPCRequest::REQUEST_DATA& ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t));
    ((uint32_t*)ResponseData.data())[0] = result;

    if (Ioctl.Out != 0 && !Request.WriteBuffer(OutData))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void INvDrvServices::NvOpen(CIPCRequest & Request)
{
    CSystemThread * thread = Request.SwitchSystem().SystemThread()->GetSystemThreadPtr();
    if (thread == nullptr) 
    { 
        g_Notify->BreakPoint(__FILE__, __LINE__); 
        return; 
    }
    CSystemThreadMemory & ThreadMemory = thread->ThreadMemory();

    if (Request.SendBuff().size() == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__); 
        return; 
    }
    uint64_t NameAddr = Request.SendBuff()[0].Address;

    std::string Name;
    if (!ThreadMemory.ReadCString(NameAddr, Name))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__); 
        return;
    }
    uint32_t fd = m_NvDriver.Open(Name);
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint64_t));
    *((uint32_t *)&ResponseData[0]) = fd;
    *((uint32_t *)&ResponseData[4]) = nvResult_Success;
}

void INvDrvServices::NvQueryEvent(CIPCRequest & Request)
{
	if (Request.RequestData().size() < 8)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    uint32_t EventId = *(uint32_t *)(&Request.RequestData()[4]) & 0x000000FF;

    CIPCRequest::REQUEST_DATA& ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t));
    if (EventId < CNvEvents::MaxEvents)
    {
        CKernelObjectPtr Event = m_NvDriver.Events().GetEvent(EventId);
        Event->GetKEventPtr()->Clear();
        CHleKernel& HleKernel = Request.SwitchSystem().HleKernel();
        uint32_t handle = HleKernel.AddKernelObject(Event);
        Request.AddResponseHandlesToCopy(handle);
        *((uint32_t*)&ResponseData[0]) = nvResult_Success;
    }
    else
    {
        *((uint32_t*)&ResponseData[0]) = nvResult_BadParameter;
    }
}

void INvDrvServices::NvSetAruid(CIPCRequest & Request)
{
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t));
    *((uint32_t *)&ResponseData[0]) = nvResult_Success;
}
