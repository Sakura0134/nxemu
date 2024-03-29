#include <nxemu-core\hle\Memory\SystemThreadMemory.h>
#include <nxemu-core\SystemGlobals.h>

CSystemThreadMemory::CSystemThreadMemory(CProcessMemory &ProcessMemory, IVideo & Video) :
    m_ProcessMemory(ProcessMemory),
    m_Video(Video),
    m_stackmem(nullptr),
    m_StackAddress(0),
    m_StackSize(0),
    m_tlsAddress(0),
    m_tlsSize(0),
    m_tls(nullptr)
{
}

CSystemThreadMemory::~CSystemThreadMemory()
{
    if (m_stackmem)
    {
        delete m_stackmem;
        m_stackmem = nullptr;
    }
    if (m_tls)
    {
        delete m_tls;
        m_tls = nullptr;
    }
}

bool CSystemThreadMemory::Initialize(uint64_t StackTop, uint32_t StackSize, uint64_t TlsAddress, uint32_t TlsSize)
{
    m_StackAddress = StackTop - StackSize;
    m_StackSize = StackSize;
    if (m_StackSize != 0)
    {
        m_stackmem = new uint8_t[m_StackSize];
        if (m_stackmem == nullptr)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        memset(m_stackmem, 0, m_StackSize);
    }

    m_tlsAddress = TlsAddress;
    m_tlsSize = TlsSize;
    if (m_tlsSize > 0)
    {
        m_tls = new uint8_t[m_tlsSize];
        if (m_tls == nullptr)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        memset(m_tls, 0, m_tlsSize);
    }
    return true;
}

bool CSystemThreadMemory::Read8(uint64_t Addr, uint8_t & value)
{
    return ReadBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CSystemThreadMemory::Read16(uint64_t Addr, uint16_t & value)
{
    return ReadBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CSystemThreadMemory::Read32(uint64_t Addr, uint32_t & value)
{
    return ReadBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CSystemThreadMemory::Read64(uint64_t Addr, uint64_t & value)
{
    return ReadBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CSystemThreadMemory::ReadBytes(uint64_t Addr, uint8_t * buffer, uint32_t len)
{
    void * ReadBuffer = nullptr;
    MemoryState State = MemoryState_None;
    if (!FindAddressMemory(Addr, len, ReadBuffer, State))
    {
        return false;
    }
    if (State == MemoryState_RasterizerMemory)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    memcpy(buffer, ReadBuffer, len);
    return true;
}

bool CSystemThreadMemory::ReadCString(uint64_t Addr, std::string & value)
{
    MemoryRegionMap & ProcessMemory = m_ProcessMemory.m_MemoryMap;
    MemoryRegionMap::const_iterator itr = ProcessMemory.lower_bound(Addr);
    if (itr != ProcessMemory.end() && Addr >= itr->second.Address() && Addr <= itr->first)
    {
        uint64_t StartIndex = Addr - itr->second.Address();
        for (uint64_t index = StartIndex, endIndex = itr->first - itr->second.Address(); index < endIndex; index++)
        {
            uint8_t * Memory = itr->second.Memory();
            if (Memory[index] != 0)
            {
                continue;
            }
            value = std::string((const char *)&Memory[StartIndex], index - StartIndex);
            return true;
        }
    }

    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool CSystemThreadMemory::Write8(uint64_t Addr, uint8_t value)
{
    return WriteBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CSystemThreadMemory::Write16(uint64_t Addr, uint16_t value)
{
    return WriteBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CSystemThreadMemory::Write32(uint64_t Addr, uint32_t value)
{
    return WriteBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CSystemThreadMemory::Write64(uint64_t Addr, uint64_t value)
{
    return WriteBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CSystemThreadMemory::WriteBytes(uint64_t Addr, const uint8_t * buffer, uint32_t len)
{
    void * WriteBuffer = nullptr;
    MemoryState State = MemoryState_None;
    if (!FindAddressMemory(Addr, len, WriteBuffer, State))
    {
        return false;
    }
    if (State == MemoryState_RasterizerMemory)
    {
        m_Video.InvalidateRegion(Addr, len);
    }
    memcpy(WriteBuffer, buffer, len);
    return true;
}

bool CSystemThreadMemory::FindAddressMemory(uint64_t Addr, uint32_t len, void *& buffer, MemoryState& State)
{
    if (Addr >= m_StackAddress && (Addr + len) <= (m_StackAddress + m_StackSize))
    {
        buffer = (void *)&m_stackmem[Addr - m_StackAddress];
        State = MemoryState_UnmanagedMemory;
        return true;
    }

    if (Addr >= m_tlsAddress && (Addr + len) <= (m_tlsAddress + m_tlsSize))
    {
        buffer = (void *)&m_tls[Addr - m_tlsAddress];
        State = MemoryState_UnmanagedMemory;
        return true;
    }

    MemoryRegionMap & ProcessMemory = m_ProcessMemory.m_MemoryMap;
    MemoryRegionMap::const_iterator itr = ProcessMemory.lower_bound(Addr);
    if (itr != ProcessMemory.end() && Addr >= itr->second.Address() && Addr <= itr->first)
    {
        if (itr->first + 1 < Addr + len)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }

        const CMemoryRegion & region = itr->second;
        switch (region.State())
        {
        case MemoryState_AllocatedMemory:
        case MemoryState_UnmanagedMemory:
        case MemoryState_RasterizerMemory:
            if (region.Memory() == nullptr)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            buffer = (void *)&region.Memory()[Addr - itr->second.Address()];
            break;
        case MemoryState_BackingMemory:
            buffer = (void *)&region.BackingMemory()[Addr - itr->second.Address()];
            break;
        case MemoryState_None:
            return false;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        State = region.State();
        return true;
    }
    return false;
}

bool CSystemThreadMemory::GetMemoryInfo(uint64_t Addr, QueryMemoryInfo & MemoryInfo)
{
    uint64_t StackEnd = m_StackAddress + m_StackSize;
    uint64_t tlsEnd = m_tlsAddress + m_tlsSize;
    if (Addr >= m_StackAddress && Addr < StackEnd)
    {
        MemoryInfo.Type = MemoryType_MappedMemory;
        MemoryInfo.BaseAddress = m_StackAddress;
        MemoryInfo.Size = m_StackSize;
        MemoryInfo.Permission = MemoryPermission_ReadWrite;
    }
    else if (Addr >= m_tlsAddress && Addr < tlsEnd)
    {
        MemoryInfo.Type = MemoryType_ThreadLocal;
        MemoryInfo.BaseAddress = m_tlsAddress;
        MemoryInfo.Size = m_tlsSize;
        MemoryInfo.Permission = MemoryPermission_ReadWrite;
    }
    else
    {
        if (!m_ProcessMemory.GetMemoryInfo(Addr, MemoryInfo))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (MemoryInfo.BaseAddress < m_StackAddress && Addr >= StackEnd)
        {
            MemoryInfo.Size -= StackEnd - MemoryInfo.BaseAddress;
            MemoryInfo.BaseAddress = StackEnd;
        }
        if (MemoryInfo.BaseAddress < m_tlsAddress && Addr >= tlsEnd)
        {
            MemoryInfo.Size -= tlsEnd - MemoryInfo.BaseAddress;
            MemoryInfo.BaseAddress = tlsEnd;
        }
        if (MemoryInfo.BaseAddress < m_StackAddress && MemoryInfo.BaseAddress + MemoryInfo.Size > m_StackAddress)
        {
            MemoryInfo.Size = m_StackAddress - MemoryInfo.BaseAddress;
        }
        if (MemoryInfo.BaseAddress < m_tlsAddress && MemoryInfo.BaseAddress + MemoryInfo.Size > m_tlsAddress)
        {
            MemoryInfo.Size = m_tlsAddress - MemoryInfo.BaseAddress;
        }
    }
    return true;
}

bool CSystemThreadMemory::IsValidAddress(uint64_t Addr)
{
    if (Addr >= m_StackAddress && Addr < (m_StackAddress + m_StackSize))
    {
        return true;
    }
    else if (Addr >= m_tlsAddress && Addr < (m_tlsAddress + m_tlsSize))
    {
        return true;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}
