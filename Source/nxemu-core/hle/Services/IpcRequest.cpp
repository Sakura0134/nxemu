#include <nxemu-core\hle\Services\IpcRequest.h>
#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\StdString.h>

CIPCRequest::CIPCRequest(CSwitchSystem & System, uint64_t RequestAddress, CService * Service) :
	m_System(System),
    m_RequestAddress(RequestAddress),
	m_Service(Service)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

bool CIPCRequest::WriteResponse(ResultCode /*call_result*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return true;
}

const char * CIPCRequest::CommandTypeName(IPC_COMMAND_TYPE Id)
{
    switch (Id)
    {
    case Command_Invalid: return "Invalid";
    case Command_LegacyRequest: return "LegacyRequest";
    case Command_Close: return "Close";
    case Command_LegacyControl: return "LegacyControl";
    case Command_Request: return "Request";
    case Command_Control: return "Control";
    case Command_RequestWithContext: return "RequestWithContext";
    case Command_ControlWithContext: return "ControlWithContext";
    }

    static stdstr unknown;
    unknown.Format("Unknown (0x%X)", Id);
    return unknown.c_str();
}
