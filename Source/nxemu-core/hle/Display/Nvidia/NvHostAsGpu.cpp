#include <nxemu-core\hle\Display\Nvidia\NvHostAsGpu.h>
#include <nxemu-core\SystemGlobals.h>

CNvHostAsGpu::CNvHostAsGpu(CNvDriver& NvDriver) :
    m_NvDriver(NvDriver)
{
}

nvResult CNvHostAsGpu::Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nvResult_Success;
}