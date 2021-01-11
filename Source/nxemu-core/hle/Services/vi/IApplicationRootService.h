#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class ServiceManger;

class IApplicationRootService :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IApplicationRootService"; }

private:
    IApplicationRootService(void);
    IApplicationRootService(const IApplicationRootService&);
    IApplicationRootService& operator=(const IApplicationRootService&);

    IApplicationRootService(CSwitchSystem & System);
};