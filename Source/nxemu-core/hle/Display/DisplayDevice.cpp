#include <nxemu-core\hle\Display\DisplayDevice.h>
#include <nxemu-core\SystemGlobals.h>

CDisplayDevice::CDisplayDevice(uint32_t id, const char * Name) :
    m_Id(id),
    m_Name(Name)
{
}

CDisplayDevice::~CDisplayDevice()
{
}