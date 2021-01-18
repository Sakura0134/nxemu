#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>
#include <nxemu-core\hle\Display\Nvidia\NvHostCtrlGpu.h>
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

private:
    CNvDriver();
    CNvDriver(const CNvDriver&);
    CNvDriver& operator=(const CNvDriver&);

    DeviceList m_Devices;
    OpenDeviceList m_Open;
    uint32_t m_NextFD;
    CNvHostCtrlGpu m_NvHostCtrlGpu;
    CSwitchSystem & m_System;
};