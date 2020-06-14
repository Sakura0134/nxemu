#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\lm\lm.h>
#include <nxemu-core\hle\Services\apm\apm.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CServiceManger::CServiceManger(CSwitchSystem & System) :
    CService(System),
    m_connected(false),
    m_Initialized(false)
{
	m_ServiceCreateList.insert(ServiceCreateList::value_type("apm", APM::CreateInstance));
    m_ServiceCreateList.insert(ServiceCreateList::value_type("lm", LM::CreateInstance));
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
    const CIPCRequest::REQUEST_DATA & data = Request.RequestData();

    switch (Request.RequestHeader().Command)
    {
    case Method::Initialize:
        m_Initialized = true;
        break;
    case Method::GetService:
        if (data.size() >= 0x8)
        {
            std::string name((const char *)data.data(), 8);
            ServiceCreateList::const_iterator itr = m_ServiceCreateList.find(name.c_str());
            if (itr != m_ServiceCreateList.end())
            {
                CHleKernel & HleKernel = Request.SwitchSystem().HleKernel();
                Request.AddResponseHandlesToMove(HleKernel.AddKernelObject(itr->second(Request.SwitchSystem())));
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}