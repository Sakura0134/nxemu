#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IApplicationDisplayService :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IApplicationDisplayService"; }

private:
    IApplicationDisplayService(void);
    IApplicationDisplayService(const IApplicationDisplayService&);
    IApplicationDisplayService& operator=(const IApplicationDisplayService&); 
    
    IApplicationDisplayService(CSwitchSystem & System);
};