#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IParentalControlService :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IParentalControlService"; }

private:
    IParentalControlService(void);
    IParentalControlService(const IParentalControlService&);
    IParentalControlService& operator=(const IParentalControlService&);

    IParentalControlService(CSwitchSystem & System);
};