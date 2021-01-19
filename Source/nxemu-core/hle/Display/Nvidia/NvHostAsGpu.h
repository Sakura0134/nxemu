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

	struct IoctlVaRegion 
	{
		uint64_t Offset;
		uint32_t PageSize;
		uint32_t Unknown;
		uint64_t Pages;
	};

	struct IoctlGetVaRegions 
	{
		uint64_t buf_addr; // (contained output user ptr on linux, ignored)
		uint32_t buf_size; // forced to 2*sizeof(struct va_region)
		uint32_t reserved;
		IoctlVaRegion regions[2];
	};

public:
    CNvHostAsGpu(CNvDriver& NvDriver);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvHostAsGpu();
    CNvHostAsGpu(const CNvHostAsGpu&);
    CNvHostAsGpu& operator=(const CNvHostAsGpu&);

    void GetVaRegions(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    void InitializeEx(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    
    CNvDriver & m_NvDriver;
};
