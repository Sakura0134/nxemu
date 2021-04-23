#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\hle\Services\audio\IAudioOut.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\Util.h>

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
    case Method_AppendAudioOutBuffer: ProcessAppendAudioOutBuffer(Request); break;
    case Method_RegisterBufferEvent: ProcessRegisterBufferEvent(Request); break;
    case Method_GetReleasedAudioOutBuffers: ProcessGetReleasedAudioOutBuffers(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}

void IAudioOut::ProcessAppendAudioOutBuffer(CIPCRequest & /*Request*/)
{
    //Stub
}

void IAudioOut::ProcessGetReleasedAudioOutBuffers(CIPCRequest & Request)
{
    nxutil::Sleep(10000);
    uint64_t MaxCount{ Request.GetWriteBufferSize() / sizeof(uint64_t) };
    std::vector<uint64_t> ReleasedBuffers;

    std::vector<uint64_t> Tags(ReleasedBuffers);
    Tags.resize(MaxCount);
    Request.WriteBuffer((uint8_t *)Tags.data(), (uint32_t)(Tags.size() * sizeof(uint64_t)));
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t));
    ((uint32_t *)ResponseData.data())[0] = (uint32_t)ReleasedBuffers.size();    
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
