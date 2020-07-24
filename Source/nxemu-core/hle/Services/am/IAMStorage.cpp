#include <nxemu-core\hle\Services\am\IAMStorage.h>
#include <nxemu-core\hle\Services\am\IStorageAccessor.h>
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
	switch (Request.RequestHeader().Command)
	{
	case Method::Open:
		ProcessOpen(Request);
		break;
	default:
		g_Notify->BreakPoint(__FILE__, __LINE__);
	}
    return RESULT_SUCCESS;
}

void IAMStorage::ProcessOpen(CIPCRequest & Request)
{
	Request.MakeObject(new IStorageAccessor(m_System, this));
}
