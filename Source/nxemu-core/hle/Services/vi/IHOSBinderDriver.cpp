#include <nxemu-core\hle\Services\vi\IHOSBinderDriver.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IHOSBinderDriver::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IHOSBinderDriver(System));
}

IHOSBinderDriver::IHOSBinderDriver(CSwitchSystem & System) :
    CService(System)
{
}

IHOSBinderDriver::~IHOSBinderDriver()
{
}

bool IHOSBinderDriver::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IHOSBinderDriver::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_AdjustRefcount:
        ViAdjustRefcount();
        break;
    case Method_GetNativeHandle:
        ViGetNativeHandle(Request);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}

void IHOSBinderDriver::ViAdjustRefcount(void)
{
    //stubbed
}

void IHOSBinderDriver::ViGetNativeHandle(CIPCRequest & Request)
{
    const CIPCRequest::REQUEST_DATA & RequestData = Request.RequestData();
    if (RequestData.size() < 0x8)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    uint32_t Id = *((uint32_t *)&RequestData[0]);
    CDisplay & Display = m_System.HleKernel().GetDisplay();
    CBufferQueue * BufferQueue = Display.FindBufferQueue(Id);
    if (BufferQueue == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    CHleKernel & HleKernel = Request.SwitchSystem().HleKernel();
    uint32_t handle = HleKernel.AddKernelObject(BufferQueue->WaitEvent());
    Request.AddResponseHandlesToCopy(handle);
}
