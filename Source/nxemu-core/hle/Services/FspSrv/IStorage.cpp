#include <nxemu-core\hle\Services\FspSrv\IStorage.h>
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
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}
