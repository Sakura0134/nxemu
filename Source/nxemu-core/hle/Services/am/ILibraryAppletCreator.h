#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class ILibraryAppletCreator :
    public CService
{
public:
    enum Method
    {
        CreateLibraryApplet = 0,
        TerminateAllLibraryApplets = 1,
        AreAnyLibraryAppletsLeft = 2,
        CreateStorage = 10,
        CreateTransferMemoryStorage = 11,
        CreateHandleStorage = 12,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "ILibraryAppletCreator"; }

private:
    ILibraryAppletCreator(void);
    ILibraryAppletCreator(const ILibraryAppletCreator&);
    ILibraryAppletCreator& operator=(const ILibraryAppletCreator&);

    ILibraryAppletCreator(CSwitchSystem & System);
};
