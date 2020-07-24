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

ResultCode IStorageAccessor::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
