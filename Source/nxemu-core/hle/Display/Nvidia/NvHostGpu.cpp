#include <nxemu-core\hle\Display\Nvidia\NvHostGpu.h>
#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>
#include <nxemu-core\SystemGlobals.h>

CNvHostGpu::CNvHostGpu(CNvDriver & NvDriver) :
    m_NvDriver(NvDriver)
{
}

nvResult CNvHostGpu::Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData)
{
    if (Ioctl.Group == nvIoctl::NvGpuMagic)
    {
        switch (Ioctl.Cmd)
        {
        case CHANNEL_SET_USER_DATA:
            SetUserData(InData, OutData);
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else if (Ioctl.Group == nvIoctl::NvHostMagic)
    {
        switch (Ioctl.Cmd)
        {
        case CHANNEL_SET_NVMAP_FD:
            SetNvMap(InData, OutData);
            break;
        case CHANNEL_SUBMIT_GPFIFO:
            SubmitGpfifo(InData, OutData);
            break;
        case CHANNEL_ALLOC_OBJ_CTX:
            AllocObjCtx(InData, OutData);
            break;
        case CHANNEL_ZCULL_BIND:
            ZcullBind(InData, OutData);
            break;
        case CHANNEL_SET_ERROR_NOTIFIER:
            SetErrorNotifier(InData, OutData);
            break;
        case CHANNEL_SET_PRIORITY:
            SetPriority(InData, OutData);
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

void CNvHostGpu::SetUserData(const CIPCRequest::RequestBuffer & /*InData*/, CIPCRequest::RequestBuffer & /*OutData*/)
{
    //Stubbed
}

void CNvHostGpu::SetPriority(const CIPCRequest::RequestBuffer& /*InData*/, CIPCRequest::RequestBuffer& /*OutData*/)
{
    //Stubbed
}

void CNvHostGpu::SubmitGpfifo(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData)
{
    if (InData.size() < sizeof(IoctlSubmitGpfifo))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    IoctlSubmitGpfifo Params;
    memcpy(&Params, InData.data(), sizeof(IoctlSubmitGpfifo));

    if (InData.size() != sizeof(IoctlSubmitGpfifo) + Params.NumEntries * sizeof(uint64_t))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    uint32_t SyncPointValue = m_NvDriver.Video().GetSyncPointValue(Params.FenceOut.id);
    Params.FenceOut.value = Params.Flags.Increment != 0 ? Params.FenceOut.value + SyncPointValue : SyncPointValue;

    m_NvDriver.Video().PushGPUEntries((const uint64_t *)&InData[sizeof(IoctlSubmitGpfifo)], Params.NumEntries);
    OutData.resize(sizeof(Params));
    std::memcpy(OutData.data(), &Params, sizeof(Params));
}

void CNvHostGpu::ZcullBind(const CIPCRequest::RequestBuffer & /*InData*/, CIPCRequest::RequestBuffer & /*OutData*/)
{
    //Stubbed
}
