#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\SystemGlobals.h>

CService::CService(CSwitchSystem & System) :
    m_System(System),
	m_Domain(false)
{
}

CService::~CService()
{
}

