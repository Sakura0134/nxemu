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
            for (uint32_t i = 0; i < m_HandleDesc.NumberOfHandlesToCopy; i++)
            {
                uint32_t handle = 0;
                if (!m_ThreadMemory.ReadBytes(ipc_read_addr, (uint8_t *)&handle, sizeof(handle)))
                {
                    g_Notify->BreakPoint(__FILE__, __LINE__);
                    return;
                }
                ipc_read_addr += sizeof(handle);
                m_RequestHandlesToCopy.push_back(handle);
            }
        }
        if (m_HandleDesc.NumberOfHandlesToMove != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }

    ReadBuffXList(ipc_read_addr, m_PointerBuff, m_cmd.num_buf_x_descriptors);
    ReadBuffList(ipc_read_addr, m_SendBuff, m_cmd.num_buf_a_descriptors);
    ReadBuffList(ipc_read_addr, m_ReceiveBuff, m_cmd.num_buf_b_descriptors);
    if (m_cmd.num_buf_w_descriptors != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); }
    uint64_t RecvListPos = ipc_read_addr + m_cmd.data_size * sizeof(uint32_t);

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
        if (data_size < sizeof(IpcDomainMessage))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        if (!m_ThreadMemory.ReadBytes(ipc_read_addr, (uint8_t *)&m_DomainMessage, sizeof(m_DomainMessage)))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        ipc_read_addr += sizeof(m_DomainMessage);
        data_size -= sizeof(m_DomainMessage);
        if (m_DomainMessage.InputObjectCount != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
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
        if (m_cmd.buf_c_descriptor_flags < 3)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        else
        {
            ReadRecvBuffList(RecvListPos, m_RecvBuffList, m_cmd.buf_c_descriptor_flags - 2);
        }
    }
    m_valid = true;
}

void CIPCRequest::AddResponseHandlesToCopy(uint32_t handle)
{
	m_ResponseHandlesToCopy.push_back(handle);
}

void CIPCRequest::AddResponseHandlesToMove(uint32_t handle)
{
    m_ResponseHandlesToMove.push_back(handle);
}

void CIPCRequest::AddObjectID(uint32_t handle)
{
	m_ObjectIds.push_back(handle);
}

bool CIPCRequest::WriteResponse(ResultCode call_result)
{
    uint64_t ipc_write_addr = m_RequestAddress;
    uint32_t objectIds = 0;

	if (IsDomainRequest() && CommandType() == Command_Request && DomainMessage().Command == 2)
	{
		//Close object, nothing to write back
		return true;
	}
    if (IsDomainRequest() && CommandType() == Command_Request && m_ObjectIds.size() > 0)
    {
        if (m_ResponseData.size() != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        m_ResponseData.resize((m_ObjectIds.size() * sizeof(uint32_t)));
        objectIds = (uint32_t)m_ObjectIds.size();
        uint8_t * data_ptr = (uint8_t *)m_ResponseData.data();
        for (size_t i = 0; i < m_ObjectIds.size(); i++)
        {
            uint32_t objectId = m_ObjectIds[i];
            memcpy(data_ptr, &objectId, sizeof(objectId));
            data_ptr += sizeof(objectId);
        }
    }

    uint32_t header_size = (uint32_t)sizeof(CIPCRequest::IpcMessageCmd);
    CIPCRequest::IpcMessageCmd CmdResponse = { 0 };
    if (m_ResponseHandlesToMove.size() > 0 || m_ResponseHandlesToCopy.size() > 0)
    {
		uint32_t handleCount = (uint32_t)(m_ResponseHandlesToMove.size() + m_ResponseHandlesToCopy.size());
        CmdResponse.enable_handle_descriptor = 1;
        header_size += (uint32_t)((handleCount * sizeof(uint32_t)) + sizeof(CIPCRequest::IpcHandleDesc));
    }
    uint32_t PrePadding = GetPadSize16((uint32_t)(ipc_write_addr + header_size));
    uint32_t PostPadding = 0x10 - PrePadding;
    uint32_t ResponseHeaderSize = IsDomainRequest() ? sizeof(IpcResponseHeader) + 0x10 : sizeof(IpcResponseHeader);

    CmdResponse.data_size = (ResponseHeaderSize + m_ResponseData.size() + PrePadding + PostPadding) / sizeof(uint32_t);
    if (!m_ThreadMemory.WriteBytes(ipc_write_addr, (uint8_t*)&CmdResponse, sizeof(CmdResponse)))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    ipc_write_addr += sizeof(CIPCRequest::IpcMessageCmd);
    if (m_ResponseHandlesToMove.size() > 0 || m_ResponseHandlesToCopy.size())
    {
        IpcHandleDesc HandleDesc = { 0 };
        HandleDesc.NumberOfHandlesToCopy = m_ResponseHandlesToCopy.size();
        HandleDesc.NumberOfHandlesToMove = m_ResponseHandlesToMove.size();
        HandleDesc.SendCurrentPID = 0;
        if (!m_ThreadMemory.WriteBytes(ipc_write_addr, (uint8_t*)&HandleDesc, sizeof(HandleDesc)))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        ipc_write_addr += sizeof(HandleDesc);
		for (size_t i = 0, n = m_ResponseHandlesToCopy.size(); i < n; i++)
		{
			if (!m_ThreadMemory.Write32(ipc_write_addr, m_ResponseHandlesToCopy[i]))
			{
				g_Notify->BreakPoint(__FILE__, __LINE__);
				return false;
			}
			ipc_write_addr += sizeof(uint32_t);
		}
		for (size_t i = 0, n = m_ResponseHandlesToMove.size(); i < n; i++)
		{
			if (!m_ThreadMemory.Write32(ipc_write_addr, m_ResponseHandlesToMove[i]))
			{
				g_Notify->BreakPoint(__FILE__, __LINE__);
				return false;
			}
			ipc_write_addr += sizeof(uint32_t);
		}
	}
    static uint8_t PaddingData[16] = { 0 };
    if (PrePadding != 0)
    {
        if (!m_ThreadMemory.WriteBytes(ipc_write_addr, PaddingData, PrePadding))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        ipc_write_addr += PrePadding;
    }

    if (IsDomainRequest())
    {
		if (!m_ThreadMemory.WriteBytes(ipc_write_addr, (uint8_t*)&objectIds, sizeof(objectIds)))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        ipc_write_addr += sizeof(objectIds);
        uint32_t PaddingSize = 0x10 - sizeof(objectIds);
        if (!m_ThreadMemory.WriteBytes(ipc_write_addr, PaddingData, PaddingSize))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        ipc_write_addr += PaddingSize;
    }

    IpcResponseHeader response;
    response.Magic = SFCO;
    response.Result = call_result.Raw;
    if (!m_ThreadMemory.WriteBytes(ipc_write_addr, (uint8_t*)&response, sizeof(response)))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    ipc_write_addr += sizeof(response);
    if (m_ResponseData.size() > 0)
    {
        if (!m_ThreadMemory.WriteBytes(ipc_write_addr, m_ResponseData.data(), (uint32_t)m_ResponseData.size()))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        ipc_write_addr += m_ResponseData.size();
    }
    return true;
}

void CIPCRequest::ReadBuffList(uint64_t & read_addr, IpcBuffDescList & list, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        IpcBuffDescPack BuffDescPack = { 0 };
        if (!m_ThreadMemory.ReadBytes(read_addr, (uint8_t*)&BuffDescPack, sizeof(BuffDescPack)))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        read_addr += sizeof(BuffDescPack);
        IpcBuffDesc BuffDesc = { 0 };
        BuffDesc.Size = (uint64_t)BuffDescPack.Size31_0 | ((uint64_t)(BuffDescPack.p.Size35_32) << 32);
        BuffDesc.Address = BuffDescPack.Address31_0 | ((uint64_t)(BuffDescPack.p.Address35_32) << 32) | ((uint64_t)(BuffDescPack.p.Address38_36) << 36);
        BuffDesc.Flags = BuffDescPack.p.Flags;
        list.push_back(BuffDesc);
    }
}

void CIPCRequest::ReadBuffXList(uint64_t & read_addr, IpcPointerDescList & list, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        IpcPointerDescPack PointerDescPack = { 0 };
        if (!m_ThreadMemory.ReadBytes(read_addr, (uint8_t*)&PointerDescPack, sizeof(PointerDescPack)))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        read_addr += sizeof(PointerDescPack);
        IpcPointerDesc BuffDesc = { 0 };
		BuffDesc.Address = PointerDescPack.Address31_0 | ((uint64_t)(PointerDescPack.p.Address35_32) << 32) | ((uint64_t)(PointerDescPack.p.Address38_36) << 36);
        BuffDesc.Size = PointerDescPack.p.Size;
        if (PointerDescPack.p.Counter11_9 != 0 ||
            PointerDescPack.p.Counter5_0 != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        list.push_back(BuffDesc);
    }
}

void CIPCRequest::ReadRecvBuffList(uint64_t & read_addr, IpcRecvBuffDescList & list, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        IpcRecvBuffDescPack PointerDescPack = { 0 };
        if (!m_ThreadMemory.ReadBytes(read_addr, (uint8_t*)&PointerDescPack, sizeof(PointerDescPack)))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        read_addr += sizeof(PointerDescPack);
        IpcRecvBuffDesc BuffDesc = { 0 };
        BuffDesc.Address = PointerDescPack.Address31_0 | ((uint64_t)(PointerDescPack.p.Address47_32) << 32);
        BuffDesc.Size = PointerDescPack.p.Size;
        list.push_back(BuffDesc);
    }
}

void CIPCRequest::SetDomainRequest(bool DomainRequest)
{
	m_IsDomainRequest = DomainRequest;
}

void CIPCRequest::MakeObject(CService * Service)
{
    if (IsDomainRequest())
	{
		AddObjectID(m_Service->GetServicePtr()->AddDomainObject(Service));
	}
	else
	{
		AddResponseHandlesToMove(m_System.HleKernel().AddKernelObject(Service));
	}
}

uint32_t CIPCRequest::GetPadSize16(uint32_t address)
{
    return (address & 0xf) != 0 ? 0x10 - (address & 0xf) : 0;
}

uint64_t CIPCRequest::GetWriteBufferSize(uint32_t BufferIndex)
{
	if (m_ReceiveBuff.size() > BufferIndex && m_ReceiveBuff[BufferIndex].Size > 0)
	{
		return m_ReceiveBuff[BufferIndex].Size;
	}
	if (m_RecvBuffList.size() > BufferIndex && m_RecvBuffList[BufferIndex].Size > 0)
	{
		return m_RecvBuffList[BufferIndex].Size;
	}
	return 0;
}

bool CIPCRequest::ReadBuffer(std::vector<uint8_t> & Data, uint32_t BufferIndex)
{
    uint64_t DataPos = 0;
    uint32_t DataSize = 0;

    if (m_PointerBuff.size() > 0 && m_PointerBuff[BufferIndex].Address != 0 && m_PointerBuff[BufferIndex].Size > 0)
    {
        DataPos = m_PointerBuff[BufferIndex].Address;
        DataSize = m_PointerBuff[BufferIndex].Size;
    }
    else if (m_SendBuff.size() > 0 && m_SendBuff[BufferIndex].Address != 0 && m_SendBuff[BufferIndex].Size > 0)
    {
        DataPos = m_SendBuff[BufferIndex].Address;
        DataSize = (uint32_t)m_SendBuff[BufferIndex].Size;
    }
    if (DataSize == 0 || DataSize > 0x200)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    Data.resize(DataSize);
    if (!m_ThreadMemory.ReadBytes(DataPos, Data.data(), DataSize))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    return true;
}

bool CIPCRequest::WriteBuffer(const RequestBuffer& Buffer, uint32_t BufferIndex) const
{
    return WriteBuffer(Buffer.data(), (uint32_t)Buffer.size(), BufferIndex);
}

bool CIPCRequest::WriteBuffer(const uint8_t* buffer, uint32_t size, uint32_t BufferIndex) const
{
    uint64_t OutPosition = 0;
	uint32_t OutSize = 0;

	if (m_ReceiveBuff.size() > BufferIndex && m_ReceiveBuff[BufferIndex].Size > 0)
	{
		OutPosition = m_ReceiveBuff[BufferIndex].Address;
		OutSize = (uint32_t)m_ReceiveBuff[BufferIndex].Size;
	}
	else if (m_RecvBuffList.size() > BufferIndex && m_RecvBuffList[BufferIndex].Size > 0)
	{
		OutPosition = m_RecvBuffList[BufferIndex].Address;
		OutSize = (uint32_t)m_RecvBuffList[BufferIndex].Size;
	}

	if (OutPosition == 0)
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}
	if (size > OutSize)
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}

    CSystemThread * thread = m_System.SystemThread()->GetSystemThreadPtr();
	if (thread == nullptr)
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}
	CSystemThreadMemory & ThreadMemory = thread->ThreadMemory();
	if (!ThreadMemory.WriteBytes(OutPosition, buffer, size))
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}
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
