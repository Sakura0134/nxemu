#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\set\Set.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr SetServices::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new SetServices(System));
}

SetServices::SetServices(CSwitchSystem & System) :
    CService(System)
{
}

bool SetServices::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode SetServices::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}