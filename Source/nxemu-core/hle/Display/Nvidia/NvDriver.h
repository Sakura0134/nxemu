#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>
#include <nxemu-core\hle\Display\Nvidia\NvHostCtrlGpu.h>
#include <nxemu-core\hle\Display\Nvidia\NvEvents.h>
#include <Common\stdtypes.h>
#include <string>

class CSwitchSystem;

class CNvDriver
{
    typedef std::map<std::string, INvDevice&> DeviceList;
    typedef std::map<uint32_t, INvDevice&> OpenDeviceList;

public:
    CNvDriver(CSwitchSystem & System);
    ~CNvDriver();

    uint32_t Open(const std::string& Name);
    nvResult Ioctl(uint32_t Fd, nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer & OutData);

    inline CNvEvents & Events(void) { return m_Events; }

private:
    CNvDriver();
    CNvDriver(const CNvDriver&);
    CNvDriver& operator=(const CNvDriver&);

    DeviceList m_Devices;
    OpenDeviceList m_Open;
    uint32_t m_NextFD;
    CNvHostCtrlGpu m_NvHostCtrlGpu;
    CNvEvents m_Events;
    CSwitchSystem & m_System;
};