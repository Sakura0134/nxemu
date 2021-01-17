#pragma once
#include <vector>
#include <string>
#include <Common\stdtypes.h>
#include <nxemu-core\hle\Services\IpcRequest.h>

class ViParcel
{
    enum { DefaultBufferSize = 0x40 };

public:
    ViParcel();
    ViParcel(const std::vector<uint8_t> & Buffer);

    CIPCRequest::RequestBuffer Serialize();

protected:
    virtual void SerializeData() {}
    virtual void DeserializeData() {}

    void Write(const void * val, size_t valSize );

protected:
    struct Header
    {
        uint32_t DataSize;
        uint32_t DataOffset;
        uint32_t ObjectsSize;
        uint32_t ObjectsOffset;
    };

    std::vector<uint8_t> m_Buffer;
    std::size_t m_ReadIndex;
    std::size_t m_WriteIndex;
};