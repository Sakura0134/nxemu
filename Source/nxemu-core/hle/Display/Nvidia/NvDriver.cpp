#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>
#include <nxemu-core\SystemGlobals.h>

CNvDriver::CNvDriver(CSwitchSystem& System) :
    m_System(System),
    m_NextFD(1)
{
    m_Devices.insert(DeviceList::value_type("/dev/nvhost-ctrl-gpu", m_NvHostCtrlGpu));
}

CNvDriver::~CNvDriver()
{
}

uint32_t CNvDriver::Open(const std::string& Name)
{
    DeviceList::iterator itr = m_Devices.find(Name);
    if (itr == m_Devices.end())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    uint32_t fd = m_NextFD++;
    m_Open.insert(OpenDeviceList::value_type(fd, itr->second));
    return fd;
}