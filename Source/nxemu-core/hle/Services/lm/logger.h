#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class LMLogger :
    public CService
{
    enum
    {
        Method_LogMessage = 0,
    };

public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "LMLogger"; }

private:
	LMLogger(void);
    LMLogger(const LMLogger&);
    LMLogger& operator=(const LMLogger&);

    LMLogger(CSwitchSystem & System);

    void ProcessLogMessage(CIPCRequest & Request);
};
