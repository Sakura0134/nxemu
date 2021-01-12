#include <nxemu-core\hle\Services\vi\IHOSBinderDriver.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IHOSBinderDriver::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IHOSBinderDriver(System));
}

IHOSBinderDriver::IHOSBinderDriver(CSwitchSystem & System) :
    CService(System)
{
}

IHOSBinderDriver::~IHOSBinderDriver()
{
}

bool IHOSBinderDriver::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IHOSBinderDriver::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}