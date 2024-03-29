#include <nxemu-core\hle\Services\am\ICommonStateGetter.h>
#include <nxemu-core\hle\Services\am\ISelfController.h>
#include <nxemu-core\hle\Services\am\IApplicationProxy.h>
#include <nxemu-core\hle\Services\am\IAudioController.h>
#include <nxemu-core\hle\Services\am\IDebugFunctions.h>
#include <nxemu-core\hle\Services\am\IDisplayController.h>
#include <nxemu-core\hle\Services\am\IWindowController.h>
#include <nxemu-core\hle\Services\am\ILibraryAppletCreator.h>
#include <nxemu-core\hle\Services\am\IApplicationFunctions.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IApplicationProxy::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IApplicationProxy(System));
}

IApplicationProxy::IApplicationProxy(CSwitchSystem & System) :
    CService(System)
{
}

bool IApplicationProxy::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IApplicationProxy::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method::GetCommonStateGetter:
        Request.MakeObject(ICommonStateGetter::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method::GetSelfController:
        Request.MakeObject(ISelfController::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method::GetWindowController:
        Request.MakeObject(IWindowController::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method::GetAudioController:
        Request.MakeObject(IAudioController::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method::GetDisplayController:
        Request.MakeObject(IDisplayController::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method::GetLibraryAppletCreator:
        Request.MakeObject(ILibraryAppletCreator::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method::GetApplicationFunctions:
        Request.MakeObject(IApplicationFunctions::CreateInstance(m_System)->GetServicePtr());
        break;
    case Method::GetDebugFunctions:
        Request.MakeObject(IDebugFunctions::CreateInstance(m_System)->GetServicePtr());
        break;        
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}
