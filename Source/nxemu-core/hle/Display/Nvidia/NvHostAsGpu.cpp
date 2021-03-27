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

