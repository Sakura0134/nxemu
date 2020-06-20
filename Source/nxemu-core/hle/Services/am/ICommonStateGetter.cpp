#include <nxemu-core\hle\Services\am\ICommonStateGetter.h>
#include <nxemu-core\hle\Kernel\KEvent.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr ICommonStateGetter::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new ICommonStateGetter(System));
}

ICommonStateGetter::ICommonStateGetter(CSwitchSystem & System) :
    CService(System),
    m_ReadEvent(new KEvent)
{
    m_ReadEvent->GetKEventPtr()->Signal();
}

bool ICommonStateGetter::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode ICommonStateGetter::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case GetEventHandle: ProcessGetEventHandle(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}

void ICommonStateGetter::ProcessGetEventHandle(CIPCRequest & Request)
{
	CHleKernel & HleKernel = Request.SwitchSystem().HleKernel();
	uint32_t handle = HleKernel.AddKernelObject(m_ReadEvent);
	Request.AddResponseHandlesToCopy(handle);
}
