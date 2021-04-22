#pragma once
#include <nxemu-core\hle\Kernel\Service.h>
#include <string>

class AudOutU :
    public CService
{
    enum Method
    {
        Method_ListAudioOuts = 0,
        Method_OpenAudioOut = 1,
        Method_ListAudioOutsAuto = 2,
        Method_OpenAudioOutAuto = 3,
    };

public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    void Close(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "AudOutU"; }

private:
    AudOutU(void);
    AudOutU(const AudOutU&);
    AudOutU& operator=(const AudOutU&);

    AudOutU(CSwitchSystem & System);

    void ProcessOpenAudioOut(CIPCRequest & Request);

    std::string m_DefaultDevice;
};