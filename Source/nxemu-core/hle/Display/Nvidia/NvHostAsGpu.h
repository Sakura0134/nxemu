#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class CNvDriver;

class CNvHostAsGpu :
    public INvDevice
{
    enum
    {
        IOCTL_BIND_CHANNEL = 0x01,
        IOCTL_ALLOC_SPACE = 0x02,
        IOCTL_MAP_BUFFER_EX = 0x06,
        IOCTL_GET_VA_REGIONS = 0x08,
        IOCTL_INITIALIZE_EX = 0x09,
    };

public:
    CNvHostAsGpu(CNvDriver& NvDriver);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvHostAsGpu();
    CNvHostAsGpu(const CNvHostAsGpu&);
    CNvHostAsGpu& operator=(const CNvHostAsGpu&);

    void InitializeEx(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    
    CNvDriver & m_NvDriver;
};
