#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IAudioOut :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System, uint32_t SampleRate, uint16_t ChannelCount);

    //__interface IService
    bool Connect(void);
    void Close(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IAudioOut"; }

private:
    IAudioOut(void);
    IAudioOut(const IAudioOut&);
    IAudioOut& operator=(const IAudioOut&);

    IAudioOut(CSwitchSystem & System, uint32_t SampleRate, uint16_t ChannelCount);

    uint32_t m_SampleRate;
    uint16_t m_ChannelCount;
};