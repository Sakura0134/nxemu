#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>
#include <nxemu-core\SystemGlobals.h>

CNvDriver::CNvDriver(CSwitchSystem& System) :
    m_System(System)
{
}

CNvDriver::~CNvDriver()
{
}

uint32_t CNvDriver::Open(const std::string & /*Name*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}