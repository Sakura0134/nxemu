#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class LM :
    public CService
{
public:
    enum Method
    {
        OpenLogger = 0,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "lm"; }

private:
    LM(void);
    LM(const LM&);
    LM& operator=(const LM&);

    LM(CSwitchSystem & System);
};