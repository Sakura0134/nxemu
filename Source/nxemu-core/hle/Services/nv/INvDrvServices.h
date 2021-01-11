#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class INvDrvServices :
    public CService
{
public:
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
};