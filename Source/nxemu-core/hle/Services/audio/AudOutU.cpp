#include <nxemu-core\hle\Services\audio\AudOutU.h>
#include <nxemu-core\hle\Services\audio\IAudioOut.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr AudOutU::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new AudOutU(System));
}

AudOutU::AudOutU(CSwitchSystem & System) :
    CService(System),
    m_DefaultDevice("DeviceOut")
{
}

bool AudOutU::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode AudOutU::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_OpenAudioOut: ProcessOpenAudioOut(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}

void AudOutU::ProcessOpenAudioOut(CIPCRequest & Request)
{
    CHleKernel & HleKernel = Request.SwitchSystem().HleKernel();
    const CIPCRequest::REQUEST_DATA & RequestData = Request.RequestData();
    
    struct AudOutUParams
    {
        uint32_t SampleRate;
        uint16_t ChannelCount;
        uint16_t padding;
    };

    if (RequestData.size() < sizeof(AudOutUParams))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    AudOutUParams & Param = *((AudOutUParams *)RequestData.data());
    Param.ChannelCount = Param.ChannelCount <= 2 ? 2 : 6;

    std::vector<uint8_t> ReadData;
    if (!Request.ReadBuffer(ReadData))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    std::string DeviceName = ReadData[0] != '\0' ? (const char *)ReadData.data() : m_DefaultDevice;
    Request.WriteBuffer((const uint8_t *)DeviceName.data(), (uint32_t)DeviceName.size());
    Request.AddResponseHandlesToMove(HleKernel.AddKernelObject(IAudioOut::CreateInstance(m_System, Param.SampleRate, Param.ChannelCount)));

    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t) * 4);
    ((uint32_t *)ResponseData.data())[0] = 48000; // DefaultSampleRate;
    ((uint32_t *)ResponseData.data())[1] = Param.ChannelCount;
    ((uint32_t *)ResponseData.data())[2] = 2; // PcmFormat;
    ((uint32_t *)ResponseData.data())[3] = 1; // AudioState-Stopped;
}
