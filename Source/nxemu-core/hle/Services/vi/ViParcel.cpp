#include <nxemu-core\hle\Services\vi\ViParcel.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\align.h>

ViParcel::ViParcel() :
    m_ReadIndex(0),
    m_WriteIndex(0)
{
    m_Buffer.resize(DefaultBufferSize);
}

void ViParcel::Write(const void * val, size_t valSize)
{
    if (m_Buffer.size() < m_WriteIndex + valSize) 
    {
        m_Buffer.resize(m_Buffer.size() + valSize + DefaultBufferSize);
    }

    std::memcpy(m_Buffer.data() + m_WriteIndex, val, valSize);
    m_WriteIndex += valSize;
    m_WriteIndex = Align::Up(m_WriteIndex, 4);
}

std::vector<uint8_t> ViParcel::Serialize()
{
    if (m_ReadIndex != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        std::vector<uint8_t> empty;
        return empty;
    }
    m_WriteIndex = sizeof(Header);

    SerializeData();

    Header header = { 0 };
    header.DataSize = (uint32_t)(m_WriteIndex - sizeof(Header));
    header.DataOffset = sizeof(Header);
    header.ObjectsSize = 4;
    header.ObjectsOffset = sizeof(Header) + header.DataSize;
    std::memcpy(m_Buffer.data(), &header, sizeof(Header));
    return m_Buffer;
}