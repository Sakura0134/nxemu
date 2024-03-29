#include <nxemu-core\hle\Services\am\IApplicationFunctions.h>
#include <nxemu-core\hle\Services\am\IAMStorage.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IApplicationFunctions::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IApplicationFunctions(System));
}

IApplicationFunctions::IApplicationFunctions(CSwitchSystem & System) :
    CService(System)
{
}

bool IApplicationFunctions::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IApplicationFunctions::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
	case Method_PopLaunchParameter: ProcessPopLaunchParameter(Request); break;
    case Method_EnsureSaveData: ProcessEnsureSaveData(Request); break;
    case Method_GetDesiredLanguage: ProcessGetDesiredLanguage(Request); break;
    case Method_NotifyRunning: ProcessNotifyRunning(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void IApplicationFunctions::ProcessPopLaunchParameter(CIPCRequest & Request)
{
	enum
	{
		MagicValue = 0xC79497CA,
	};

	struct LaunchParameters
	{
		uint32_t Magic;
		uint32_t AccountSelected;
		uint64_t current_user[2];
		uint8_t padding[0x70];
	} params = { 0 };

	params.Magic = (uint32_t)MagicValue;
	params.AccountSelected = 1;
	params.current_user[0] = 0x5d3bb8826610c28e;
	params.current_user[1] = 0x77a43cb14da4c0b4;

	Request.MakeObject(IAMStorage::CreateInstance(m_System, (uint8_t*)&params, sizeof(params))->GetServicePtr());
}

void IApplicationFunctions::ProcessGetDesiredLanguage(CIPCRequest & Request)
{
	CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
	ResponseData.resize(sizeof(uint64_t));

    const uint64_t EN_US = 0x00000053552D6E65;
	((uint64_t *)ResponseData.data())[0] = EN_US;
}

void IApplicationFunctions::ProcessEnsureSaveData(CIPCRequest & Request)
{
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint64_t));
    ((uint64_t *)ResponseData.data())[0] = 0;
}

void IApplicationFunctions::ProcessNotifyRunning(CIPCRequest & Request)
{
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t));
    ((uint32_t *)ResponseData.data())[0] = 0;
}

