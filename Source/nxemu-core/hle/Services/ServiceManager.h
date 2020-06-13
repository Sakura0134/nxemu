#pragma once
#include <nxemu-core\hle\Services\NamedPort.h>
#include <nxemu-core\hle\Kernel\Service.h>

class CSwitchSystem;

class CServiceManger :
    public CService
{
    enum Method
    {
        Initialize = 0,
        GetService = 1,
        RegisterService = 2,
        UnregisterService = 3,
    };

    typedef CKernelObjectPtr(*CreateService) (CSwitchSystem & System);
    typedef std::map<std::string, CreateService> ServiceCreateList;

public:
	CServiceManger(CSwitchSystem & System);

    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "sm"; }
    CSwitchSystem & System(void) { return m_System; }

private:
	CServiceManger(void);                              // Disable default constructor
	CServiceManger(const CServiceManger&);             // Disable copy constructor
	CServiceManger& operator=(const CServiceManger&);  // Disable assignment
	
    ServiceCreateList m_ServiceCreateList;
    bool m_connected;
    bool m_Initialized;
};