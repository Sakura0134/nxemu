#include <nxemu-core\hle\Services\vi\ViParcel.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\align.h>

ViParcel::ViParcel() :
    m_ReadIndex(0),
    m_WriteIndex(0)
{
    m_Buffer.resize(DefaultBufferSize);
}

ViParcel::ViParcel(const std::vector<uint8_t> & Buffer) :
    m_ReadIndex(0),
    m_WriteIndex(0),
    m_Buffer(Buffer)
{
}

void ViParcel::ReadUnaligned(void * dst, size_t size)
{
    if (m_ReadIndex + size > m_Buffer.size())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    memcpy(dst, m_Buffer.data() + m_ReadIndex, size);
    m_ReadIndex += size;
}

void ViParcel::Read(void * dst, size_t size)
{
    ReadUnaligned(dst, size);
    m_ReadIndex = Align::Up(m_ReadIndex, 4);
}

std::wstring ViParcel::ReadInterfaceToken()
{
    uint32_t Reserved, Length;
    
    Read(&Reserved, sizeof(Reserved));
    Read(&Length, sizeof(Length));

    std::wstring token;
    for (uint32_t i = 0, n = Length + 1; i < n; i++)
    {
        wchar_t c;
        ReadUnaligned(&c, sizeof(c));
        token.push_back(c);
    }

    m_ReadIndex = Align::Up(m_ReadIndex, 4);
    return token;
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

void ViParcel::WriteObject(const void * val, size_t valSize)
{
    uint32_t Value0 = 0, Size = (uint32_t)valSize;

    Write(&Size, sizeof(Size));
    Write(&Value0, sizeof(Value0));
    Write(val, valSize);
}

const CIPCRequest::RequestBuffer & ViParcel::Serialize()
{
    if (m_ReadIndex != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        static std::vector<uint8_t> empty;
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

void ViParcel::Deserialize()
{
    if (m_Buffer.size() < sizeof(Header))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    Header header;
    memcpy(&header, m_Buffer.data(), sizeof(Header));

    m_ReadIndex = header.DataOffset;
    DeserializeData();
}
