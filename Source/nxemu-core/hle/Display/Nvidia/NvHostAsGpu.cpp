#include <nxemu-core\Plugins\VideoPlugin.h>
#include <nxemu-core\hle\Display\Nvidia\NvHostAsGpu.h>
#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>
#include <nxemu-core\SystemGlobals.h>

CNvHostAsGpu::CNvHostAsGpu(CNvDriver& NvDriver) :
    m_NvDriver(NvDriver)
{
}

nvResult CNvHostAsGpu::Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData)
{
    if (Ioctl.Group == nvIoctl::NvGpuAsMagic)
    {
        switch (Ioctl.Cmd)
        {
        case IOCTL_ALLOC_SPACE:
            AllocSpace(InData, OutData);
            break;
        case IOCTL_MAP_BUFFER_EX:
            MapBufferEx(InData, OutData);
            break;
        case IOCTL_GET_VA_REGIONS:
            GetVaRegions(InData, OutData);
            break;
        case IOCTL_INITIALIZE_EX:
            InitializeEx(InData, OutData);
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return nvResult_Success;
}

void CNvHostAsGpu::GetVaRegions(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData)
{
    if (InData.size() < sizeof(IoctlGetVaRegions))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    OutData.resize(sizeof(IoctlGetVaRegions));
    memcpy(OutData.data(), InData.data(), InData.size());
    IoctlGetVaRegions & params = *((IoctlGetVaRegions *)OutData.data());
    
    params.buf_size = 0x30;
	params.regions[0].Offset = 0x04000000;
	params.regions[0].PageSize = 0x1000;
	params.regions[0].Pages = 0x3fbfff;

	params.regions[1].Offset = 0x04000000;
	params.regions[1].PageSize = 0x10000;
	params.regions[1].Pages = 0x1bffff;
}

void CNvHostAsGpu::InitializeEx(const std::vector<uint8_t>& /*InData*/, std::vector<uint8_t>& /*OutData*/)
{
    //stubbed
}

void CNvHostAsGpu::AllocSpace(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData)
{
    if (InData.size() < sizeof(NvGpuASAllocSpace))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    OutData.resize(sizeof(NvGpuASAllocSpace));
    memcpy(OutData.data(), InData.data(), InData.size());
    NvGpuASAllocSpace & Params = *((NvGpuASAllocSpace *)OutData.data());
    IVideo & Video = m_NvDriver.Video();
    uint64_t Size = (uint64_t)Params.Pages * (uint64_t)Params.PageSize;

    if ((Params.Flags & AddressSpaceFlags_FixedOffset) != AddressSpaceFlags_None)
    {
        Params.Offset = Video.VideoMemoryAllocateFixed(Params.Offset, Size);
    }
    else 
    {
        Params.Offset = Video.VideoMemoryAllocate(Size, Params.Align);
    }
}

void CNvHostAsGpu::MapBufferEx(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData)
{
    if (InData.size() < sizeof(NvGpuASAllocSpace))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    OutData.resize(sizeof(NvGpuASMapBufferEx));
    memcpy(OutData.data(), InData.data(), InData.size());
    NvGpuASMapBufferEx & Params = *((NvGpuASMapBufferEx *)OutData.data());
    if (Params.NvmapHandle == 0)
    {
        return;
    }
    CNvMap & NvMap = m_NvDriver.NvMap();
    NvMapHandle * Map = NvMap.FindNvMapHandle(Params.NvmapHandle);
    if (Map == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    uint64_t PhysicalAddress = Map->Address() + Params.BufferOffset;
    uint64_t Size = Params.MappingSize != 0 ? Params.MappingSize : Map->Size();
    uint64_t PageSize = Params.PageSize != 0 ? Params.PageSize : Map->Align();

    bool IsAlloc = (Params.Flags & AddressSpaceFlags_FixedOffset) == AddressSpaceFlags_None;
    bool Remap = (Params.Flags & AddressSpaceFlags_Remap) != AddressSpaceFlags_None;

    IVideo & Video = m_NvDriver.Video();
    if (Remap)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    if (IsAlloc)
    {
        Params.Offset = Video.VideoMemoryMapAllocate(PhysicalAddress, Size, PageSize);
    }
    else 
    {
        Video.VideoMemoryMap(PhysicalAddress, Params.Offset, Size);
    }

    if (Params.Offset == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    AddBufferMap(Params.Offset, Size, PhysicalAddress, IsAlloc);
}

void CNvHostAsGpu::AddBufferMap(uint64_t GpuAddr, uint64_t Size, uint64_t CpuAddr, bool Allocated)
{
    m_BufferMappings.emplace(std::make_pair(GpuAddr, BufferMap(GpuAddr, Size, CpuAddr, Allocated)));
}

CNvHostAsGpu::BufferMap::BufferMap(uint64_t StartAddr, uint64_t Size, uint64_t CpuAddr, bool Allocated) :
    m_StartAddr(StartAddr),
    m_EndAddr(StartAddr + Size),
    m_CpuAddr(CpuAddr),
    Allocated(Allocated)
{
}