#pragma once
#include <Common\stdtypes.h>
#include <string>

class CSwitchSystem;

class CNvDriver
{
public:
    CNvDriver(CSwitchSystem & System);
    ~CNvDriver();

    uint32_t Open(const std::string& Name);

private:
    CNvDriver();
    CNvDriver(const CNvDriver&);
    CNvDriver& operator=(const CNvDriver&);

    CSwitchSystem & m_System;
};