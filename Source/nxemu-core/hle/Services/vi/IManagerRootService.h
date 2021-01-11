#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IManagerRootService :
    public CService
{
public:
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