#include <nxemu-core\hle\Display\Display.h>
#include <nxemu-core\SystemGlobals.h>

CDisplay::CDisplay(CSwitchSystem & System) :
    m_System(System),
    m_NextLayerId(1),
    m_NextBufferQueueId(1)
{
    m_DisplayList.push_back(std::make_shared<CDisplayDevice>(0, "Default"));
}

CDisplay::~CDisplay()
{
    m_DisplayList.clear();
    m_BufferQueue.clear();
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

bool CDisplay::CreateLayer(uint32_t DisplayId, uint32_t & LayerId)
{
    CDisplayDevice * display = FindDisplay(DisplayId);

    if (display == nullptr) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    LayerId = m_NextLayerId++;
    const uint32_t BufferQueueId = m_NextBufferQueueId++;
    std::shared_ptr<CBufferQueue> BufferQueue = std::make_shared<CBufferQueue>(BufferQueueId, LayerId);
    m_BufferQueue.push_back(BufferQueue);
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

CDisplayDevice * CDisplay::FindDisplay(uint32_t /*DisplayId*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}
