#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class CNvDriver;

class CNvHostCtrl :
    public INvDevice
{
public:
    CNvHostCtrl(CNvDriver& NvDriver);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);

private:
    CNvHostCtrl(void);
    CNvHostCtrl(const CNvHostCtrl&);
    CNvHostCtrl& operator=(const CNvHostCtrl&);

    CNvDriver& m_NvDriver;
};
