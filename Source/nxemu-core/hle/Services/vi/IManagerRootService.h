#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IManagerRootService :
    public CService
{
public:
    enum Method
    {
        Method_GetDisplayService = 2,
        Method_GetDisplayServiceWithProxyNameExchange = 3,
    };
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IManagerRootService"; }

private:
    IManagerRootService(void);
    IManagerRootService(const IManagerRootService&);
    IManagerRootService& operator=(const IManagerRootService&);

    IManagerRootService(CSwitchSystem & System);
};