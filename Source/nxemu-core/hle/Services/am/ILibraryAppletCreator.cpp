#include <nxemu-core\hle\Services\am\ILibraryAppletCreator.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr ILibraryAppletCreator::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new ILibraryAppletCreator(System));
}

ILibraryAppletCreator::ILibraryAppletCreator(CSwitchSystem & System) :
    CService(System)
{
}

bool ILibraryAppletCreator::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode ILibraryAppletCreator::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
