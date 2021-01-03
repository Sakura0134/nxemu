#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class SSLServices :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "ssl"; }

private:
    SSLServices(void);
    SSLServices(const SSLServices&);
    SSLServices& operator=(const SSLServices&);

    SSLServices(CSwitchSystem & System);
};