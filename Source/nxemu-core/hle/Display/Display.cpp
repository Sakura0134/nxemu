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
    return display->CreateLayer(LayerId, BufferQueue);
}

bool CDisplay::FindBufferQueueId(uint32_t DisplayId, uint32_t LayerId, uint32_t & QueueId)
{
    CDisplayDevice * DisplayDevice = FindDisplay(DisplayId);
    if (DisplayDevice == nullptr)
    {
        return false;
    }
    CLayer * Layer = DisplayDevice->FindLayer(LayerId);
    if (Layer == nullptr)
    {
        return false;
    }
    QueueId = Layer->BufferQueue()->Id();
    return true;
}

CBufferQueue * CDisplay::FindBufferQueue(uint32_t QueueId)
{
    for (size_t i = 0, n = m_BufferQueue.size(); i < n; i++)
    {
        if (m_BufferQueue[i]->Id() == QueueId)
        {
            return m_BufferQueue[i].get();
        }
    }
    return nullptr;
}

CDisplayDevice * CDisplay::FindDisplay(uint32_t DisplayId)
{
    for (size_t i = 0, n = m_DisplayList.size(); i < n; i++)
    {
        if (m_DisplayList[i]->Id() == DisplayId)
        {
            return m_DisplayList[i].get();
        }
    }
    return nullptr;
}