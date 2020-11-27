#include <nxemu-core\hle\Services\FspSrv\IStorage.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IStorage::CreateInstance(CSwitchSystem & System, CEncryptedFile * File, uint64_t Offset)
{
    return CKernelObjectPtr(new IStorage(System, File, Offset));
}

IStorage::IStorage(CSwitchSystem & System, CEncryptedFile * File, uint64_t Offset) :
    CService(System),
	m_File(File),
	m_Offset(Offset)
{
}

bool IStorage::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IStorage::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method::Read:
        ProcessRead(Request);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void IStorage::ProcessRead(CIPCRequest & Request)
{
	const CIPCRequest::REQUEST_DATA & data = Request.RequestData();
    if (data.size() < 0x16)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    const CIPCRequest::IpcBuffDescList & ReceiveBuff = Request.ReceiveBuff();
    if (ReceiveBuff.size() < 1)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    const CIPCRequest::IpcBuffDesc & BuffDesc = ReceiveBuff[0];

    if (*((uint32_t *)&data[12]) != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); return; }

    uint64_t Offset = *((uint64_t *)&data[0]);
    uint32_t Size = *((uint32_t *)&data[8]);
    if (Size > BuffDesc.Size)
    {
        Size = (uint32_t)BuffDesc.Size;
    }

    std::vector<uint8_t> Data(Size);
	if(m_File->Seek(m_Offset + Offset, CFileBase::begin) < 0)
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return;
	}

    if (!m_File->Read(Data.data(), Size, Offset))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    CSystemThread * thread = Request.SwitchSystem().SystemThread()->GetSystemThreadPtr();
    if (thread == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    CSystemThreadMemory & ThreadMemory = thread->ThreadMemory();
    if (!ThreadMemory.WriteBytes(BuffDesc.Address, Data.data(), Size))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
}
