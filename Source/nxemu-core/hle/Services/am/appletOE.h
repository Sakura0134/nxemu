#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class appletOE :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);
    
    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "appletOE"; }

private:
    appletOE(void);
    appletOE(const appletOE&); 
    appletOE& operator=(const appletOE&);

    appletOE(CSwitchSystem & System);
};
