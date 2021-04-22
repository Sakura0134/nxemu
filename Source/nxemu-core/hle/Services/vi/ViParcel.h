#pragma once
#include <nxemu-core\hle\Services\IpcRequest.h>
#include <vector>
#include <string>
#include <stdint.h>

class ViParcel
{
    enum { DefaultBufferSize = 0x40 };

public:
    ViParcel();
    ViParcel(const std::vector<uint8_t> & Buffer);

    const CIPCRequest::RequestBuffer & Serialize();
    void Deserialize();

protected:
    virtual void SerializeData() {}
    virtual void DeserializeData() {}

    std::wstring ReadInterfaceToken();
    void Write(const void * Value, size_t Size );
    void WriteObject(const void * val, size_t valSize);
    void Read(void * Buffer, size_t Size);
    void ReadUnaligned(void * dst, size_t size);

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