#include <nxemu-core\hle\Services\lm\logger.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>

CKernelObjectPtr LMLogger::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new LMLogger(System));
}

LMLogger::LMLogger(CSwitchSystem & System) :
    CService(System)
{
}

bool LMLogger::Connect(void)
{
    return true;
}

ResultCode LMLogger::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_LogMessage: ProcessLogMessage(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void LMLogger::ProcessLogMessage(CIPCRequest & /*Request*/)
{
    //Stubbed;
}
