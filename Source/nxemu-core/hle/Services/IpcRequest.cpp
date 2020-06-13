#include <nxemu-core\hle\Services\IpcRequest.h>
#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\StdString.h>

CIPCRequest::CIPCRequest(CSwitchSystem & System, uint64_t RequestAddress, CService * Service) :
	m_System(System),
    m_RequestAddress(RequestAddress),
	m_Service(Service),
	m_ThreadMemory(System.SystemThread()->GetSystemThreadPtr()->ThreadMemory()),
    m_PId(0),
    m_cmd({ 0 }),
    m_HandleDesc({ 0 }),
    m_RequestHeader({ 0 }),
    m_DomainMessage({ 0 }),
    m_IsDomainRequest(Service->IsDomain()),
    m_valid(false)
{
    uint64_t ipc_read_addr = RequestAddress;
    if (!m_ThreadMemory.ReadBytes(ipc_read_addr, (uint8_t*)&m_cmd, sizeof(m_cmd)))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    ipc_read_addr += sizeof(CIPCRequest::IpcMessageCmd);
    WriteTrace(TraceHleKernel, TraceInfo, "CommandType: %s data_size: 0x%X enable_handle_descriptor: %s", CommandTypeName(CommandType()), m_cmd.data_size, m_cmd.enable_handle_descriptor ? "true" : "false");

    if (m_cmd.enable_handle_descriptor != 0)
    {
        if (!m_ThreadMemory.ReadBytes(ipc_read_addr, (uint8_t*)&m_HandleDesc, sizeof(m_HandleDesc)))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        ipc_read_addr += sizeof(IpcHandleDesc);
        if (m_HandleDesc.SendCurrentPID != 0)
        {
            if (!m_ThreadMemory.ReadBytes(ipc_read_addr, (uint8_t *)&m_PId, sizeof(m_PId)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            ipc_read_addr += sizeof(m_PId);
        }
        if (m_HandleDesc.NumberOfHandlesToCopy != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (m_HandleDesc.NumberOfHandlesToMove != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }

    if (m_cmd.num_buf_x_descriptors != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); }
    if (m_cmd.num_buf_a_descriptors != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); }
    if (m_cmd.num_buf_b_descriptors != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); }
    if (m_cmd.num_buf_w_descriptors != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); }

    uint32_t data_size = m_cmd.data_size * sizeof(uint32_t);
    if (data_size > 0x100)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    ipc_read_addr += GetPadSize16((uint32_t)ipc_read_addr); // data padding
    if (data_size < sizeof(IpcRequestHeader) && CommandType() != Command_Close)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    if (m_IsDomainRequest && CommandType() == Command_Request)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    if (!m_IsDomainRequest || m_DomainMessage.Command != 2)
    {
        if (data_size >= sizeof(IpcRequestHeader))
        {
            if (!m_ThreadMemory.ReadBytes(ipc_read_addr, (uint8_t*)&m_RequestHeader, sizeof(m_RequestHeader)))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            if (m_RequestHeader.Magic != SFCI)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            ipc_read_addr += sizeof(m_RequestHeader);
            data_size -= sizeof(m_RequestHeader);
        }
        if (data_size > 0)
        {
            m_RequestData.resize(data_size);
            if (!m_ThreadMemory.ReadBytes(ipc_read_addr, &m_RequestData[0], data_size))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            ipc_read_addr += data_size;
        }
    }
    if (m_cmd.buf_c_descriptor_flags != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_valid = true;
}

bool CIPCRequest::WriteResponse(ResultCode /*call_result*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return true;
}

uint32_t CIPCRequest::GetPadSize16(uint32_t address)
{
    return (address & 0xf) != 0 ? 0x10 - (address & 0xf) : 0;
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
