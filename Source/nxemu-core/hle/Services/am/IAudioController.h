#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IAudioController :
    public CService
{
public:
    enum Method
    {
        SetExpectedMasterVolume = 0,
        GetMainAppletExpectedMasterVolume = 1,
        GetLibraryAppletExpectedMasterVolume = 2,
        ChangeMainAppletMasterVolume = 3,
        SetTransparentVolumeRate = 4,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IAudioController"; }

private:
    IAudioController(void);
    IAudioController(const IAudioController&);
    IAudioController& operator=(const IAudioController&);

    IAudioController(CSwitchSystem & System);
};
