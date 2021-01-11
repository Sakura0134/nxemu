#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class ServiceManger;

class ISystemRootService :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "ISystemRootService"; }

private:
    ISystemRootService(void);
    ISystemRootService(const ISystemRootService&);
    ISystemRootService& operator=(const ISystemRootService&);

    ISystemRootService(CSwitchSystem & System);
};