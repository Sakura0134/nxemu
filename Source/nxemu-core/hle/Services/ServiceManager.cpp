#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CServiceManger::CServiceManger(CSwitchSystem & System) :
    CService(System),
    m_connected(false)
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

ResultCode CServiceManger::CallMethod(CIPCRequest & /*Request*/)
{    
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}