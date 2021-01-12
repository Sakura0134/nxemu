#include <nxemu-core\hle\Display\Display.h>
#include <nxemu-core\SystemGlobals.h>

CDisplay::CDisplay(CSwitchSystem & System) :
    m_System(System)
{
}

CDisplay::~CDisplay()
{
}

bool CDisplay::Open(const char * name, uint32_t & DisplayId)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

