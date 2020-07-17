#include <nxemu-core\hle\Services\FspSrv\IFileSystemProxy.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IFileSystemProxy::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IFileSystemProxy(System));
}

IFileSystemProxy::IFileSystemProxy(CSwitchSystem & System) :
    CService(System)
{
}

bool IFileSystemProxy::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IFileSystemProxy::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
	return RESULT_SUCCESS;
}

