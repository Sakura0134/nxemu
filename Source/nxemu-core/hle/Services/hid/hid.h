#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class HID :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    void Close(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "hid"; }

private:
    HID(void);
    HID(const HID&);
    HID& operator=(const HID&);

    HID(CSwitchSystem & System);
};