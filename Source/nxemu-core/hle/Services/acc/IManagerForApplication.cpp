#include <nxemu-core\hle\Services\acc\IManagerForApplication.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IManagerForApplication::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IManagerForApplication(System));
}

IManagerForApplication::IManagerForApplication(CSwitchSystem & System) :
    CService(System)
{
}

bool IManagerForApplication::Connect(void)
{
	g_Notify->BreakPoint(__FILE__, __LINE__);
	return false;
}

ResultCode IManagerForApplication::CallMethod(CIPCRequest & /*Request*/)
{
	g_Notify->BreakPoint(__FILE__, __LINE__);
	return RESULT_SUCCESS;
}
