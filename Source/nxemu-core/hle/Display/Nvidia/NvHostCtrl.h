#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class CNvDriver;

class CNvHostCtrl :
    public INvDevice
{
    enum
    {        
        NVCTRL_IOCTL_EVENT_WAIT_ASYNC_COMMAND = 0x1E,
        NVCTRL_IOCTL_EVENT_REGISTER_COMMAND = 0x1F,
    };

    struct CtrlWaitParams 
    {
        uint32_t SyncptId;
        uint32_t Threshold;
        uint32_t Timeout;
        uint32_t Value;
    };

    struct CtrlEventRegisterParams 
    {
        uint32_t UserEventId;
    };

public:
    CNvHostCtrl(CNvDriver& NvDriver);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);

private:
    CNvHostCtrl(void);
    CNvHostCtrl(const CNvHostCtrl&);
    CNvHostCtrl& operator=(const CNvHostCtrl&);

    nvResult EventRegister(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void EventWait(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData, bool Async);

    CNvDriver& m_NvDriver;
};
