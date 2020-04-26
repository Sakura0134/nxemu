#include <nxemu-core\hle\Memory\SystemThreadMemory.h>
#include <nxemu-core\SystemGlobals.h>

CSystemThreadMemory::CSystemThreadMemory(CProcessMemory &ProcessMemory, CPUExecutor * Executor) :
    m_ProcessMemory(ProcessMemory),
    m_StackBase(0),
    m_StackSize(0),
    m_stackmem(NULL),
    m_Executor(Executor)
{
}

CSystemThreadMemory::~CSystemThreadMemory()
{
    if (m_stackmem)
    {
        delete m_stackmem;
        m_stackmem = NULL;
    }
}

bool CSystemThreadMemory::Initialize(uint64_t StackTop, uint32_t StackSize)
{
    m_StackBase = StackTop - StackSize;
    m_StackSize = StackSize;
    if (m_StackSize != 0)
    {
        m_stackmem = new uint8_t[m_StackSize];
        if (m_stackmem == NULL)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        memset(m_stackmem, 0, m_StackSize);
    }
    return true;
}

bool CSystemThreadMemory::Read8(uint64_t Addr, uint8_t & value)
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
    void * ReadBuffer = NULL;
    if (!FindAddressMemory(Addr, len, ReadBuffer))
    {
        return false;
    }
    memcpy(buffer, ReadBuffer, len);
    return true;
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
    void * WriteBuffer = NULL;
    if (!FindAddressMemory(Addr, len, WriteBuffer))
    {
        return false;
    }
    memcpy(WriteBuffer, buffer, len);
    return true;
}

bool CSystemThreadMemory::FindAddressMemory(uint64_t Addr, uint32_t len, void *& buffer)
{
    if (Addr >= m_StackBase && (Addr + len) < (m_StackBase + m_StackSize))
    {
        buffer = (void *)&m_stackmem[Addr - m_StackBase];
        return true;
    }

    MemoryMap & ProcessMemory = m_ProcessMemory.m_MemoryMap;
    MemoryMap::const_iterator itr = ProcessMemory.lower_bound(Addr);
    if (itr != ProcessMemory.end() && Addr >= itr->second.start_addr && Addr <= itr->first)
    {
        if (itr->first < Addr + len)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        buffer = (void *)&itr->second.memory[Addr - itr->second.start_addr];
        return true;
    }
    return false;
}

