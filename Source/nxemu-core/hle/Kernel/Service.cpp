#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\SystemGlobals.h>

CService::CService(CSwitchSystem & System) :
    m_System(System),
	m_Domain(false),
	m_NextDomanObjectId(1)
{
}

CService::~CService()
{
}

int32_t CService::AddDomainObject(CService * Service)
{
    if (Service == NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    int32_t ObjectID = m_NextDomanObjectId++;
    m_DomainObjects.insert(IServiceList::value_type(ObjectID, Service));
    return ObjectID;
}
