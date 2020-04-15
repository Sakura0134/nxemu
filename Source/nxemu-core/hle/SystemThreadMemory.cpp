#include <nxemu-core\hle\SystemThreadMemory.h>
#include <nxemu-core\SystemGlobals.h>

CSystemThreadMemory::CSystemThreadMemory(CProcessMemory &ProcessMemory, CPUExecutor * Executor) :
    m_ProcessMemory(ProcessMemory),
    m_Executor(Executor)
{
}

CSystemThreadMemory::~CSystemThreadMemory()
{
}

bool CSystemThreadMemory::Read32(uint64_t Addr, uint32_t & value)
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

