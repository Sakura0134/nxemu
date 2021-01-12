#pragma once
#include <Common\stdtypes.h>
#include <string>

class CDisplayDevice
{
public:
    CDisplayDevice(uint32_t id, const char * Name);
    ~CDisplayDevice();

    inline uint32_t Id(void) const { return m_Id; }
    inline const char * Name(void) const { return m_Name.c_str(); }

private:
    uint32_t m_Id;
    std::string m_Name;
};
