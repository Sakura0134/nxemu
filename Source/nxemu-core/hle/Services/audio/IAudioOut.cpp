#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\hle\Services\audio\IAudioOut.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IAudioOut::CreateInstance(CSwitchSystem & System, uint32_t SampleRate, uint16_t ChannelCount)
{
    return CKernelObjectPtr(new IAudioOut(System, SampleRate, ChannelCount));
}

IAudioOut::IAudioOut(CSwitchSystem & System, uint32_t SampleRate, uint16_t ChannelCount) :
    CService(System),
    m_SampleRate(SampleRate),
    m_ChannelCount(ChannelCount)
{
}

bool IAudioOut::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IAudioOut::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
