#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IManagerForApplication :
	public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

	bool Connect(void);
	ResultCode CallMethod(CIPCRequest & Request);
	const char * Name(void) const { return "IManagerForApplication"; }

private:
	IManagerForApplication(void);
	IManagerForApplication(const IManagerForApplication&);
	IManagerForApplication& operator=(const IManagerForApplication&);

    IManagerForApplication(CSwitchSystem & System);
};
