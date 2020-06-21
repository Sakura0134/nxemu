#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IAddOnContentManager :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IAddOnContentManager"; }

private:
    IAddOnContentManager(void);
    IAddOnContentManager(const IAddOnContentManager&);
    IAddOnContentManager& operator=(const IAddOnContentManager&);

    IAddOnContentManager(CSwitchSystem & System);
};