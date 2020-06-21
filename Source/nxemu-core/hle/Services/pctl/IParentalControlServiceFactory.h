#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IParentalControlServiceFactory :
    public CService
{
public:
    enum Method
    {
        CreateService = 0,
        CreateServiceWithoutInitialize = 1
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "pctl:a"; }

private:
    IParentalControlServiceFactory(void);
    IParentalControlServiceFactory(const IParentalControlServiceFactory&);
    IParentalControlServiceFactory& operator=(const IParentalControlServiceFactory&);

    IParentalControlServiceFactory(CSwitchSystem & System);
};