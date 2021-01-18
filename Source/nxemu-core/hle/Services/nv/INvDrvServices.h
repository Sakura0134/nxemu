#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>
#include <nxemu-core\hle\Kernel\Service.h>

class INvDrvServices :
    public CService
{
public:
    enum Method
    {
        Method_Open = 0,
        Method_Ioctl = 1,
        Method_Close = 2,
        Method_Initialize = 3,
        Method_QueryEvent = 4,
        Method_MapSharedMem = 5,
        Method_GetStatus = 6,
        Method_SetAruidWithoutCheck = 7,
        Method_SetAruid = 8,
        Method_DumpStatus = 9,
        Method_InitializeDevtools = 10,
        Method_Ioctl2 = 11,
        Method_Ioctl3 = 12,
        Method_SetGraphicsFirmwareMemoryMarginEnabled = 13,
    };
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);
    ~INvDrvServices(void);

    //__interface IService
    bool Connect(void);
    void Close(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "nvdrv"; }

private:
    INvDrvServices(void);
    INvDrvServices(const INvDrvServices&); 
    INvDrvServices& operator=(const INvDrvServices&);
    
    INvDrvServices(CSwitchSystem & System);

    void NvInitialize(CIPCRequest & Request);
    void NvOpen(CIPCRequest & Request);
	void NvSetAruid(CIPCRequest & Request);

    CNvDriver & m_NvDriver;
};