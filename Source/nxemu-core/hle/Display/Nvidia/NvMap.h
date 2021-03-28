#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class CNvMap :
    public INvDevice
{
public:
    CNvMap(void);
    ~CNvMap(void);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvMap(const CNvMap&);
    CNvMap& operator=(const CNvMap&);
};
