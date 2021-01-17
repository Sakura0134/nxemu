#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IHOSBinderDriver :
    public CService
{
public:
    enum Method
    {
        Method_TransactParcel = 0,
        Method_AdjustRefcount = 1,
        Method_GetNativeHandle = 2,
        Method_TransactParcelAuto = 3,
    };
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

    void ViAdjustRefcount(void);
    void ViGetNativeHandle(CIPCRequest & Request);
    ResultCode ViTransactParcel(CIPCRequest & Request);
};