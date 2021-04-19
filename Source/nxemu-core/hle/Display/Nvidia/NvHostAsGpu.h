#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>
#include <Common\padding.h>

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

    struct NvGpuASAllocSpace
    {
        uint32_t Pages;
        uint32_t PageSize;
        uint32_t Flags;
        PADDING_WORDS(1);
        union 
        {
            uint64_t Offset;
            uint64_t Align;
        };
    };

    struct NvGpuASMapBufferEx
    {
        uint32_t Flags; 
        uint32_t Kind;
        uint32_t NvmapHandle;
        uint32_t PageSize;
        uint64_t BufferOffset;
        uint64_t MappingSize;
        uint64_t Offset;
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

    enum 
    {
        AddressSpaceFlags_None = 0x0,
        AddressSpaceFlags_FixedOffset = 0x1,
        AddressSpaceFlags_Remap = 0x100,
    };

public:
    CNvHostAsGpu(CNvDriver& NvDriver);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvHostAsGpu();
    CNvHostAsGpu(const CNvHostAsGpu&);
    CNvHostAsGpu& operator=(const CNvHostAsGpu&);

    class BufferMap 
    {
    public:
        BufferMap(uint64_t StartAddr, uint64_t Size, uint64_t CpuAddr, bool Allocated);

    private:
        BufferMap();
        BufferMap& operator=(const BufferMap&);

        uint64_t m_StartAddr;
        uint64_t m_EndAddr;
        uint64_t m_CpuAddr;
        bool Allocated;
    };
    typedef std::map<uint64_t, BufferMap> BufferMappings;

    void BindChannel(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    void AllocSpace(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    void MapBufferEx(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    void GetVaRegions(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    void InitializeEx(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    void AddBufferMap(uint64_t GpuAddr, uint64_t Size, uint64_t CpuAddr, bool IsAllocated);
    
    CNvDriver & m_NvDriver;
    BufferMappings m_BufferMappings;
};
