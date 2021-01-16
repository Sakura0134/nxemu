#include <nxemu-core\hle\Display\DisplayDevice.h>
#include <nxemu-core\SystemGlobals.h>

CDisplayDevice::CDisplayDevice(uint32_t id, const char * Name) :
    m_Id(id),
    m_Name(Name)
{
}

CDisplayDevice::~CDisplayDevice()
{
    m_layers.clear();
}

bool CDisplayDevice::CreateLayer(uint32_t id, std::shared_ptr<CBufferQueue> buffer_queue)
{
    if (!m_layers.empty())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    m_layers.push_back(std::make_shared<CLayer>(id, buffer_queue));
    return true;
}

CLayer * CDisplayDevice::FindLayer(uint32_t id)
{
    for (size_t i = 0, n = m_layers.size(); i < n; i++)
    {
        if (m_layers[i]->Id() == id)
        {
            return m_layers[i].get();
        }
    }
    return nullptr;
}
