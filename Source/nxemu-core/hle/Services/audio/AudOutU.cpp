#include <nxemu-core\hle\Services\audio\AudOutU.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr AudOutU::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new AudOutU(System));
}

AudOutU::AudOutU(CSwitchSystem & System) :
    CService(System)
{
}

bool AudOutU::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode AudOutU::CallMethod(CIPCRequest & Request)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
