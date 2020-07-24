#include <nxemu-core\hle\Services\am\IAMStorage.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IAMStorage::CreateInstance(CSwitchSystem & System, uint8_t * Data, uint64_t Size)
{
    return CKernelObjectPtr(new IAMStorage(System, Data, Size));
}

IAMStorage::~IAMStorage()
{
}

IAMStorage::IAMStorage(CSwitchSystem & System, uint8_t * Data, uint64_t Size) :
    CService(System)
{
    m_Data.resize(Size);
    memcpy(m_Data.data(), Data, Size);
}

bool IAMStorage::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IAMStorage::CallMethod(CIPCRequest & Request)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
