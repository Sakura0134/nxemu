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
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void IStorageAccessor::ProcessGetSize(CIPCRequest & Request)
{
    if (m_Storage == NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint64_t));
    *((uint64_t *)ResponseData.data()) = m_Storage->m_Data.size();
}
