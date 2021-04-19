#include <nxemu-core\hle\Display\Nvidia\NvHostGpu.h>
#include <nxemu-core\SystemGlobals.h>

CNvHostGpu::CNvHostGpu(CNvDriver & NvDriver) :
    m_NvDriver(NvDriver)
{
}

nvResult CNvHostGpu::Ioctl(nvIoctl /*Ioctl*/, const CIPCRequest::RequestBuffer & /*InData*/, CIPCRequest::RequestBuffer & /*OutData*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nvResult_Success;
}
