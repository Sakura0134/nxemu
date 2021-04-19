#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class CNvDriver;

class CNvHostGpu :
    public INvDevice
{
public:
    CNvHostGpu(CNvDriver& NvDriver);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvHostGpu(void);
    CNvHostGpu(const CNvHostGpu&);
    CNvHostGpu& operator=(const CNvHostGpu&);

    CNvDriver & m_NvDriver;
};
