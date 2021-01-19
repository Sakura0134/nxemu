#include <nxemu-core\hle\Display\Nvidia\NvHostAsGpu.h>
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

void CNvHostAsGpu::InitializeEx(const std::vector<uint8_t>& /*InData*/, std::vector<uint8_t>& /*OutData*/)
{
    //stubbed
}
