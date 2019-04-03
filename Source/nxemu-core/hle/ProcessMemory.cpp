#include <nxemu-core/hle/ProcessMemory.h>
#include <nxemu-core/SystemGlobals.h>
#include <Common/MemoryManagement.h>

CProcessMemory::CProcessMemory(void)
{
}

uint8_t * CProcessMemory::MapMemory(uint64_t Address, uint32_t Size, MemoryPermission Perm, MemoryType type)
{
    if (Size == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return NULL;
    }

    if ((Address & CPageTable::PageMask) != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return NULL;
    }

    if ((Size & CPageTable::PageMask) != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return NULL;
    }

    // Check overlap

    uint8_t * memory = (uint8_t *)AllocateAddressSpace(Size);
    if (memory == NULL || CommitMemory(memory, Size, MEM_READWRITE) == NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return NULL;
    }

    m_MemoryMap.insert(MemoryMap::value_type(Address + Size - 1, MemoryInfo{ Address, memory, type }));
    return memory;
}

bool CProcessMemory::Read32(uint64_t Addr, uint32_t & value)
{
    if ((Addr & 3) != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); }

    return ReadBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CProcessMemory::Read64(uint64_t Addr, uint64_t & value)
{
    if ((Addr & 7) != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); }

    return ReadBytes(Addr, (uint8_t *)&value, sizeof(value));
}

bool CProcessMemory::ReadBytes(uint64_t Addr, uint8_t * buffer, uint32_t len)
{
    void * ReadBuffer = NULL;
    if (!FindAddressMemory(Addr, len, ReadBuffer))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    memcpy(buffer, ReadBuffer, len);
    return true;
}

bool CProcessMemory::FindAddressMemory(uint64_t Addr, uint32_t len, void *& buffer)
{
    MemoryMap::const_iterator itr = m_MemoryMap.lower_bound(Addr);
    if (itr != m_MemoryMap.end() && Addr >= itr->second.start_addr && Addr <= itr->first)
    {
        if (itr->first < Addr + len)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        buffer = (void *)&itr->second.memory[Addr - itr->second.start_addr];
        return true;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

