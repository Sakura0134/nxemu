#include <nxemu-core\hle\Display\Display.h>
#include <nxemu-core\SystemGlobals.h>

CDisplay::CDisplay(CSwitchSystem & System) :
    m_System(System)
{
    m_DisplayList.push_back(std::make_shared<CDisplayDevice>(0, "Default"));
}

CDisplay::~CDisplay()
{
    m_DisplayList.clear();
}

bool CDisplay::Open(const char * name, uint32_t & DisplayId)
{
    for (size_t i = 0, n = m_DisplayList.size(); i < n; i++)
    {
        if (strcmp(m_DisplayList[i]->Name(), name) != 0)
        {
            continue;
        }
        DisplayId = m_DisplayList[i]->Id();
        return true;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

