#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IApplicationProxy :
    public CService
{
public:
    enum Method
    {
        GetCommonStateGetter = 0,
        GetSelfController = 1,
        GetWindowController = 2,
        GetAudioController = 3,
        GetDisplayController = 4,
        GetProcessWindingController = 10,
        GetLibraryAppletCreator = 11,
        GetApplicationFunctions = 20,
        GetDebugFunctions = 1000,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IApplicationProxy"; }

private:
    IApplicationProxy(void);
    IApplicationProxy(const IApplicationProxy&);
    IApplicationProxy& operator=(const IApplicationProxy&);

    IApplicationProxy(CSwitchSystem & System);
};
