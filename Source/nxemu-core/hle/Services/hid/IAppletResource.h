#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IAppletResource :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    void Close(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IAppletResource"; }

private:
    IAppletResource(void);
    IAppletResource(const IAppletResource&);
    IAppletResource& operator=(const IAppletResource&);

    IAppletResource(CSwitchSystem & System);
};
