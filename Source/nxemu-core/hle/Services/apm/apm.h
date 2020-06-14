#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class APM :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "apm"; }

private:
    APM(void);
    APM(const APM&);
    APM& operator=(const APM&);

    APM(CSwitchSystem & System);
};