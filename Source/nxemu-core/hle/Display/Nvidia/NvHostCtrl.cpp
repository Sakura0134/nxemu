#include <nxemu-core\hle\Display\Nvidia\NvHostCtrl.h>
#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>
#include <nxemu-core\hle\Display\Nvidia\NvEvents.h>
#include <SystemGlobals.h>

CNvHostCtrl::CNvHostCtrl(CNvDriver& NvDriver) :
    m_NvDriver(NvDriver)
{
}

nvResult CNvHostCtrl::Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData)
{
    if (Ioctl.Group == nvIoctl::NvHostCustomMagic)
    {
        switch (Ioctl.Cmd)
        {
        case NVCTRL_IOCTL_EVENT_REGISTER_COMMAND: 
            return EventRegister(InData, OutData); 
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return nvResult_Success;
}

nvResult CNvHostCtrl::EventRegister(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & /*OutData*/)
{
    const CtrlEventRegisterParams & Params = *((const CtrlEventRegisterParams *)InData.data());
    uint32_t EventId = Params.UserEventId & 0xFF;
    CNvEvents & Events = m_NvDriver.Events();

    if (EventId >= CNvEvents::MaxEvents)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nvResult_BadParameter;
    }
    if (Events.IsRegistered(EventId))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nvResult_BadParameter;
    }
    Events.Register(EventId);
    return nvResult_Success;
}