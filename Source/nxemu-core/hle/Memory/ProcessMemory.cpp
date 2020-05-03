#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\MemoryManagement.h>

CProcessMemory::CProcessMemory(void) :
    m_CodeRegionStart(0),
    m_CodeRegionSize(0),
    m_MapRegionBase(0),
    m_MapRegionSize(0),
    m_HeapRegionBase(0),
    m_HeapRegionSize(0),
    m_NewMapRegionBase(0),
    m_NewMapRegionSize(0),
    m_TlsIoRegionBase(0),
    m_TlsIoRegionSize(0)
{
}

CProcessMemory::~CProcessMemory()
{
}

bool CProcessMemory::Initialize(ProgramAddressSpaceType Type, bool Is64bit)
{
    WriteTrace(TraceMemory, TraceInfo, "Start (ProgramAddressSpaceType: %s Is64bit: %s)", ProgramAddressSpaceTypeName(Type), Is64bit ? "Yes": "No");

    if (!Is64bit)
    {
        return false;
    }

    switch (Type)
    {
    case ProgramAddressSpace_Is39Bit:
        m_CodeRegionStart = 0x8000000;
        m_CodeRegionSize = 0x80000000;
        m_MapRegionSize = 0x1000000000;
        m_HeapRegionSize = 0x180000000;
        m_NewMapRegionSize = 0x80000000;
        m_TlsIoRegionSize = 0x1000000000;
        break;
    default:
        WriteTrace(TraceMemory, TraceError, "ProgramAddressSpaceType(%s) is not handled", ProgramAddressSpaceTypeName(Type), Is64bit ? "Yes" : "No");
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: false)");
        return false;
    }

    m_MapRegionBase = m_CodeRegionStart + m_CodeRegionSize;
    m_HeapRegionBase = m_MapRegionBase + m_MapRegionSize;
    m_NewMapRegionBase = m_HeapRegionBase + m_HeapRegionSize;
    m_TlsIoRegionBase = m_NewMapRegionBase + m_NewMapRegionSize;

    WriteTrace(TraceMemory, TraceDebug, "MapRegionBase: 0x%I64X MapRegionSize: 0x%I64X", m_MapRegionBase, m_MapRegionSize);
    WriteTrace(TraceMemory, TraceDebug, "HeapRegionBase: 0x%I64X MapRegionSize: 0x%I64X", m_HeapRegionBase, m_HeapRegionSize);
    WriteTrace(TraceMemory, TraceDebug, "NewMapRegionBase: 0x%I64X MapRegionSize: 0x%I64X", m_NewMapRegionBase, m_NewMapRegionSize);
    WriteTrace(TraceMemory, TraceDebug, "TlsIoRegionBase: 0x%I64X MapRegionSize: 0x%I64X", m_TlsIoRegionBase, m_TlsIoRegionSize);

    WriteTrace(TraceMemory, TraceInfo, "Done (res: true)");
    return true;
}

uint8_t * CProcessMemory::MapMemory(uint64_t Address, uint32_t Size, MemoryPermission Perm, MemoryType Type)
{
    WriteTrace(TraceMemory, TraceInfo, "Start (Address: 0x%I64X Size: 0x%X Perm: %s)", Address, Size, MemoryPermissionName(Perm), MemoryTypeName(Type));

    if (Size == 0)
    {
        WriteTrace(TraceMemory, TraceError, "Can not map, size == 0");
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: NULL)");
        return NULL;
    }

    if ((Address & CPageTable::PageMask) != 0)
    {
        WriteTrace(TraceMemory, TraceError, "Can not map, Address not page aligned (Address: 0x%I64X PageMask: 0x%X)", Address, CPageTable::PageMask);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: NULL)");
        return NULL;
    }

    if ((Size & CPageTable::PageMask) != 0)
    {
        WriteTrace(TraceMemory, TraceError, "Can not map, Size not page aligned (Size: 0x%X PageMask: 0x%X)", Size, CPageTable::PageMask);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: NULL)");
        return NULL;
    }

    // Check overlap

    uint8_t * Memory = (uint8_t *)AllocateAddressSpace(Size);
    if (Memory == NULL || CommitMemory(Memory, Size, MEM_READWRITE) == NULL)
    {
        WriteTrace(TraceMemory, TraceError, "Failed to create memory (Size: 0x%X)", Size);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: NULL)");
        return NULL;
    }

    m_MemoryMap.insert(MemoryMap::value_type(Address + Size - 1, MemoryInfo{ Address, Memory, Type }));
    WriteTrace(TraceMemory, TraceInfo, "Done (Memory: 0x%I64X)", Memory);
    return Memory;
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

bool CProcessMemory::ReadBytes(uint64_t Address, uint8_t * buffer, uint32_t len)
{
    void * ReadBuffer = NULL;
    if (!FindAddressMemory(Address, len, ReadBuffer))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    memcpy(buffer, ReadBuffer, len);
    return true;
}

bool CProcessMemory::ReadCString(uint64_t Addr, std::string & value)
{
    MemoryMap::const_iterator itr = m_MemoryMap.lower_bound(Addr);
    if (itr != m_MemoryMap.end() && Addr >= itr->second.start_addr && Addr < itr->first)
    {
        uint64_t StartIndex = Addr - itr->second.start_addr;
        for (uint64_t index = StartIndex, endIndex = itr->first - itr->second.start_addr; index < endIndex; index++)
        {
            if (itr->second.memory[index] != 0)
            {
                continue;
            }
            value = std::string((const char *)&itr->second.memory[StartIndex], index - StartIndex);
            return true;
        }
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
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

