#include <nxemu-core\hle\Services\am\IStorageAccessor.h>
#include <nxemu-core\hle\Services\am\IAMStorage.h>
#include <nxemu-core\SystemGlobals.h>

IStorageAccessor::IStorageAccessor(CSwitchSystem & System, IAMStorage * storage) :
    CService(System),
    m_StorageService(storage),
    m_Storage(storage)
{
}

bool IStorageAccessor::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

void IStorageAccessor::Close(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

ResultCode IStorageAccessor::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_GetSize: ProcessGetSize(Request); break;
    case Method_Read: ProcessRead(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void IStorageAccessor::ProcessGetSize(CIPCRequest & Request)
{
    if (m_Storage == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint64_t));
    *((uint64_t *)ResponseData.data()) = m_Storage->m_Data.size();
}

void IStorageAccessor::ProcessRead(CIPCRequest & Request)
{
    const CIPCRequest::REQUEST_DATA & data = Request.RequestData();
    if (data.size() < 8)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    uint64_t offset = *(uint64_t *)(&data[0]);
    if (offset != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    uint64_t Size = Request.GetWriteBufferSize();
    if (Size > (m_Storage->m_Data.size() - offset))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    if (!Request.WriteBuffer(m_Storage->m_Data.data() + offset, (uint32_t)Size))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}
