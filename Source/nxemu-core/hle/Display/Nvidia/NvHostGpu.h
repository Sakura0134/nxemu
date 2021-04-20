#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class CNvDriver;

class CNvHostGpu :
    public INvDevice
{
    enum 
    {
        CHANNEL_SET_NVMAP_FD = 0x01,
        CHANNEL_ALLOC_GPFIFO_EX2 = 0x1A,
    };

public:
    CNvHostGpu(CNvDriver& NvDriver);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvHostGpu(void);
    CNvHostGpu(const CNvHostGpu&);
    CNvHostGpu& operator=(const CNvHostGpu&);

    void SetNvMap(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void AllocGpfifoEx2(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);

    CNvDriver & m_NvDriver;
};
