#include <nxemu-core\hle\Services\FspSrv\IFileSystemProxy.h>
#include <nxemu-core\hle\Services\FspSrv\IStorage.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\FileFormat\xci.h>
#include <nxemu-core\FileFormat\nca.h>
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

ResultCode IFileSystemProxy::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method::SetCurrentProcess:
        //stubbed; 
        break;
    case Method::OpenDataStorageByCurrentProcess:
		if (Request.SwitchSystem().Xci() != nullptr)
		{
			const NCA * Program = Request.SwitchSystem().Xci()->Program();
			if (Program)
			{
				Request.MakeObject(IStorage::CreateInstance(Request.SwitchSystem(), Program->Romfs(), Program->RomfsOffset())->GetServicePtr());
			}
			else
			{
				g_Notify->BreakPoint(__FILE__, __LINE__);
			}
		}
		else
		{
			g_Notify->BreakPoint(__FILE__, __LINE__);
		}
		break;
    case Method::GetGlobalAccessLogMode: 
        ProcessGetGlobalAccessLogMode(Request);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
	return RESULT_SUCCESS;
}

void IFileSystemProxy::ProcessGetGlobalAccessLogMode(CIPCRequest & Request)
{
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t));
	((uint32_t *)ResponseData.data())[0] = 1;
}
