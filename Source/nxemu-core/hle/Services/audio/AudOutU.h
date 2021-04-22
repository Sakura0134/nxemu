#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class AudOutU :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    void Close(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "AudOutU"; }

private:
    AudOutU(void);
    AudOutU(const AudOutU&);
    AudOutU& operator=(const AudOutU&);

    AudOutU(CSwitchSystem & System);
};