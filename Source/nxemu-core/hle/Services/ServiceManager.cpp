#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CServiceManger::CServiceManger(CSwitchSystem & System) :
    CService(System),
    m_connected(false),
    m_Initialized(false)
{
}

bool CServiceManger::Connect(void)
{
    if (m_connected)
    {
        return false;
    }
    m_connected = true;
    return true;
}

ResultCode CServiceManger::CallMethod(CIPCRequest & Request)
{    
    switch (Request.RequestHeader().Command)
    {
    case Method::Initialize:
        m_Initialized = true;
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}