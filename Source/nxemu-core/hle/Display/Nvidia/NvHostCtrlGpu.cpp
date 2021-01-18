#include <nxemu-core\hle\Display\Nvidia\NvHostCtrlGpu.h>
#include <nxemu-core\SystemGlobals.h>

CNvHostCtrlGpu::CNvHostCtrlGpu(void)
{
}

nvResult CNvHostCtrlGpu::Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nvResult_Success;
}