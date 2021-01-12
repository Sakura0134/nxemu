#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IHOSBinderDriver :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);
    ~IHOSBinderDriver();

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IHOSBinderDriver"; }

private:
    IHOSBinderDriver(void);
    IHOSBinderDriver(const IHOSBinderDriver&);
    IHOSBinderDriver& operator=(const IHOSBinderDriver&);
    
    IHOSBinderDriver(CSwitchSystem & System);
};