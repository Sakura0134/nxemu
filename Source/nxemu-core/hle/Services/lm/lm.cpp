#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\lm\lm.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr LM::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new LM(System));
}

LM::LM(CSwitchSystem & System) :
    CService(System)
{
}

bool LM::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode LM::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}