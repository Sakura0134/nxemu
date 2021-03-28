#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CNvDriver::CNvDriver(CSwitchSystem& System) :
    m_System(System),
    m_NextFD(1),
    m_NvHostAsGpu(*this)
{
    m_Devices.insert(DeviceList::value_type("/dev/nvhost-as-gpu", m_NvHostAsGpu));
    m_Devices.insert(DeviceList::value_type("/dev/nvhost-ctrl-gpu", m_NvHostCtrlGpu));
    m_Devices.insert(DeviceList::value_type("/dev/nvmap", m_NvMap));
}

CNvDriver::~CNvDriver()
{
}

IVideo & CNvDriver::Video()
{
    return m_System.Video();
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

nvResult CNvDriver::Ioctl(uint32_t Fd, nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData)
{
    OpenDeviceList::iterator itr = m_Open.find(Fd);
    if (itr == m_Open.end())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nvResult_Success;
    }
    return itr->second.Ioctl(Ioctl, InData, OutData);
}
