#include <nxemu-core\hle\Services\am\IAudioController.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IAudioController::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IAudioController(System));
}

IAudioController::IAudioController(CSwitchSystem & System) :
    CService(System)
{
}

bool IAudioController::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IAudioController::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
