#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class CNvDriver;

class CNvHostAsGpu :
    public INvDevice
{
public:
    CNvHostAsGpu(CNvDriver& NvDriver);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvHostAsGpu();
    CNvHostAsGpu(const CNvHostAsGpu&);
    CNvHostAsGpu& operator=(const CNvHostAsGpu&);

    CNvDriver & m_NvDriver;
};
