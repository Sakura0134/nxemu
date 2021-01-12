#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class ISystemDisplayService :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "ISystemDisplayService"; }

private:
    ISystemDisplayService(void);
    ISystemDisplayService(const ISystemDisplayService&);
    ISystemDisplayService& operator=(const ISystemDisplayService&);
    
    ISystemDisplayService(CSwitchSystem & System);
};