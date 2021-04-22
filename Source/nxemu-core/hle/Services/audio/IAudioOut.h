#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IAudioOut :
    public CService
{
    enum Method
    {
        Method_GetAudioOutState = 0,
        Method_Start = 1,
        Method_Stop = 2,
        Method_AppendAudioOutBuffer = 3,
        Method_RegisterBufferEvent = 4,
        Method_GetReleasedAudioOutBuffers = 5,
        Method_ContainsAudioOutBuffer = 6,
        Method_AppendAudioOutBufferAuto = 7,
        Method_GetReleasedAudioOutBuffersAuto = 8,
        Method_GetAudioOutBufferCount = 9,
        Method_GetAudioOutPlayedSampleCount = 10, 
        Method_FlushAudioOutBuffers = 11,
        Method_SetAudioOutVolume = 12,
        Method_GetAudioOutVolume = 13,
    };

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

    void ProcessRegisterBufferEvent(CIPCRequest & Request);
    void ProcessStart(CIPCRequest & Request);

    uint32_t m_SampleRate;
    uint16_t m_ChannelCount;
    CKernelObjectPtr m_BufferEvent;
};