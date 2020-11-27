#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\SystemGlobals.h>

CService::CService(CSwitchSystem & System) :
    m_System(System),
	m_Domain(false),
	m_ObjectId(-1),
	m_NextDomanObjectId(1)
{
}

CService::~CService()
{
}

int32_t CService::ConvertToDomain(void)
{
    if (m_ObjectId != -1)
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
	}
	m_ObjectId = AddDomainObject(this);
	m_Domain = true;
	return m_ObjectId;
}

bool CService::FindDomainService(int32_t ObjectID, CService *& Service)
{
	IServiceList::const_iterator itr = m_DomainObjects.find(ObjectID);
	if (itr == m_DomainObjects.end())
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}
	Service = itr->second->GetServicePtr();
	return true;
}

int32_t CService::AddDomainObject(CService * Service)
{
    if (Service == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    int32_t ObjectID = m_NextDomanObjectId++;
    m_DomainObjects.insert(IServiceList::value_type(ObjectID, Service));
    return ObjectID;
}
