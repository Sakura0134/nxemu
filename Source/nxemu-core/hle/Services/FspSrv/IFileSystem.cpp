#include <nxemu-core\hle\Services\FspSrv\IFileSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IFileSystem::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IFileSystem(System));
}

IFileSystem::IFileSystem(CSwitchSystem & System) :
    CService(System)
{
}

bool IFileSystem::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IFileSystem::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
	return RESULT_SUCCESS;
}

