#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IFileSystem :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IFileSystem"; }

private:
	IFileSystem(void);
	IFileSystem(const IFileSystem&);
	IFileSystem& operator=(const IFileSystem&);

    IFileSystem(CSwitchSystem & System);
};