#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>
#include <nxemu-core\hle\Display\Nvidia\NvMultiFence.h>

class CNvDriver;

class CNvHostGpu :
    public INvDevice
{
    enum 
    {
        CHANNEL_SET_NVMAP_FD = 0x01,
        CHANNEL_SUBMIT_GPFIFO = 0x08,
        CHANNEL_ALLOC_OBJ_CTX = 0x09,
        CHANNEL_ZCULL_BIND = 0x0B,
        CHANNEL_SET_ERROR_NOTIFIER = 0x0C,
        CHANNEL_SET_PRIORITY = 0x0D,
        CHANNEL_SET_USER_DATA = 0x14,
        CHANNEL_ALLOC_GPFIFO_EX2 = 0x1A,
    };

    struct IoctlSubmitGpfifo 
    {
        uint64_t Gpfifo;
        uint32_t NumEntries;
#pragma warning(push)
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
        union 
        {
            uint32_t raw;
            struct
            {
                unsigned AddWait : 1;
                unsigned AddIncrement : 1;
                unsigned NewHWFormat : 1;
                unsigned  : 5;
                unsigned Increment : 1;
            } ;
        } Flags;
#pragma warning(pop)
        NvFence FenceOut;
    };

public:
    CNvHostGpu(CNvDriver& NvDriver);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvHostGpu(void);
    CNvHostGpu(const CNvHostGpu&);
    CNvHostGpu& operator=(const CNvHostGpu&);

    void SetNvMap(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void AllocObjCtx(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void AllocGpfifoEx2(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void SetErrorNotifier(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void SetUserData(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void SetPriority(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void SubmitGpfifo(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void ZcullBind(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);

    CNvDriver & m_NvDriver;
};
