#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>

class NvMapHandle
{
public:
    NvMapHandle(uint32_t Size);

    void Allocate(int64_t Address, uint32_t Size, uint32_t Align, uint32_t Kind);

    inline bool Allocated() const { return m_Allocated; }
    inline uint32_t Size() const { return m_Size; }
    inline uint64_t Address() const { return m_Address; }
    inline uint32_t Align() const { return m_Align; }

private:
    NvMapHandle(void);
    NvMapHandle(const NvMapHandle&);
    NvMapHandle& operator=(const NvMapHandle&);

    uint64_t m_Address;
    uint32_t m_Size;
    uint32_t m_Align;
    uint32_t m_Kind;
    bool m_Allocated;
};

class CNvMap :
    public INvDevice
{
    enum
    {
        NVMAP_IOC_CREATE = 0x01,
        NVMAP_IOC_ALLOC = 0x04,
    };

    enum
    {
        PageSize = 0x1000
    };

    typedef std::map<uint32_t, NvMapHandle> NvMapHandleMap;

public:
    CNvMap(void);
    ~CNvMap(void);

    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvMap(const CNvMap&);
    CNvMap& operator=(const CNvMap&);

    void Create(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);
    void Alloc(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData);

    NvMapHandleMap m_Handles;
    uint32_t m_NextNvHandle;
};
