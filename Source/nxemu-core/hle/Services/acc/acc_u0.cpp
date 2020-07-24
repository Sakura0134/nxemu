#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\hle\Services\acc\acc_u0.h>

CKernelObjectPtr ACC_U0::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new ACC_U0(System));
}

ACC_U0::ACC_U0(CSwitchSystem & System) :
    CService(System)
{
}

bool ACC_U0::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode ACC_U0::CallMethod(CIPCRequest & Request)
{
	switch (Request.RequestHeader().Command)
	{
    case Method::InitializeApplicationInfoV0: ProcessInitializeApplicationInfo(); break;
	default:
		g_Notify->BreakPoint(__FILE__, __LINE__);
	}
	return RESULT_SUCCESS;
}

void ACC_U0::ProcessInitializeApplicationInfo(void)
{
}
