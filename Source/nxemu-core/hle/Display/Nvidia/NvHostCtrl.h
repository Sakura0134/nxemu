#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class CNvDriver;

class CNvHostCtrl :
    public INvDevice
{
    enum
    {        
        NVCTRL_IOCTL_EVENT_REGISTER_COMMAND = 0x1F,
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

    CNvDriver& m_NvDriver;
};
