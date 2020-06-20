#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IWindowController :
    public CService
{
public:
    enum Method
    {
        CreateWindow = 0,
        GetAppletResourceUserId = 1,
        GetAppletResourceUserIdOfCallerApplet = 2,
        AcquireForegroundRights = 10,
        ReleaseForegroundRights = 11,
        RejectToChangeIntoBackground = 12,
        SetAppletWindowVisibility = 20,
        SetAppletGpuTimeSlice = 21,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IWindowController"; }

private:
    IWindowController(void);
    IWindowController(const IWindowController&);
    IWindowController& operator=(const IWindowController&);

    IWindowController(CSwitchSystem & System);
};
