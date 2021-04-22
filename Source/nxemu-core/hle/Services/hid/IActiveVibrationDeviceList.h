#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IActiveVibrationDeviceList :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    void Close(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IActiveVibrationDeviceList"; }

private:
    IActiveVibrationDeviceList(void);
    IActiveVibrationDeviceList(const IActiveVibrationDeviceList&);
    IActiveVibrationDeviceList& operator=(const IActiveVibrationDeviceList&);

    IActiveVibrationDeviceList(CSwitchSystem & System);
};
