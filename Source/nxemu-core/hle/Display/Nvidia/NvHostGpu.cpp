#include <nxemu-core\hle\Display\Nvidia\NvHostGpu.h>
#include <nxemu-core\SystemGlobals.h>

CNvHostGpu::CNvHostGpu(CNvDriver & NvDriver) :
    m_NvDriver(NvDriver)
{
}

nvResult CNvHostGpu::Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData)
{
    if (Ioctl.Group == nvIoctl::NvHostMagic)
    {
        switch (Ioctl.Cmd)
        {
        case CHANNEL_SET_NVMAP_FD:
            SetNvMap(InData, OutData);
            break;
        case CHANNEL_ALLOC_OBJ_CTX:
            AllocObjCtx(InData, OutData);
            break;
        case CHANNEL_SET_ERROR_NOTIFIER:
            SetErrorNotifier(InData, OutData);
            break;
        case CHANNEL_ALLOC_GPFIFO_EX2:
            AllocGpfifoEx2(InData, OutData);
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

void CNvHostGpu::SetNvMap(const CIPCRequest::RequestBuffer & /*InData*/, CIPCRequest::RequestBuffer & /*OutData*/)
{
    //Stubbed
}

void CNvHostGpu::AllocObjCtx(const CIPCRequest::RequestBuffer & /*InData*/, CIPCRequest::RequestBuffer & /*OutData*/)
{
    //Stubbed
}

void CNvHostGpu::AllocGpfifoEx2(const CIPCRequest::RequestBuffer & /*InData*/, CIPCRequest::RequestBuffer & /*OutData*/)
{
    //Stubbed
}

void CNvHostGpu::SetErrorNotifier(const CIPCRequest::RequestBuffer & /*InData*/, CIPCRequest::RequestBuffer & /*OutData*/)
{
    //Stubbed
}

