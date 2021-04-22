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
    m_ChannelCount(ChannelCount),
    m_BufferEvent(new KEvent)
{
}

bool IAudioOut::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IAudioOut::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_Start: ProcessStart(Request); break;
    case Method_RegisterBufferEvent: ProcessRegisterBufferEvent(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}

void IAudioOut::ProcessRegisterBufferEvent(CIPCRequest & Request)
{
    CHleKernel & HleKernel = Request.SwitchSystem().HleKernel();
    Request.AddResponseHandlesToCopy(HleKernel.AddKernelObject(m_BufferEvent));
}

void IAudioOut::ProcessStart(CIPCRequest & /*Request*/)
{
    //Stub
}
