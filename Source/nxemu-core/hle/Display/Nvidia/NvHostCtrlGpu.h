#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class CNvHostCtrlGpu :
    public INvDevice
{
public:
    CNvHostCtrlGpu(void);
    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvHostCtrlGpu(const CNvHostCtrlGpu&);
    CNvHostCtrlGpu& operator=(const CNvHostCtrlGpu&);
};