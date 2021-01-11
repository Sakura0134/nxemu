#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IManagerDisplayService :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IManagerDisplayService"; }

private:
    IManagerDisplayService(void);
    IManagerDisplayService(const IManagerDisplayService&);
    IManagerDisplayService& operator=(const IManagerDisplayService&);
    
    IManagerDisplayService(CSwitchSystem & System);
};