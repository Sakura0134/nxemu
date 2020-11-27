#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\MemoryManagement.h>

const uint64_t CProcessMemory::PageBits = 12;
const uint64_t CProcessMemory::PageSize = 1 << PageBits;
const uint64_t CProcessMemory::PageMask = PageSize - 1;

CProcessMemory::CProcessMemory(void) :
    m_heap(nullptr),
    m_AddressSpaceWidth(0),
    m_AddressSpaceBase(0),
    m_AddressSpaceSize(0),
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

    if (m_MemoryMap.size() != 0)
    {
        WriteTrace(TraceMemory, TraceError, "Memory map is not empty (size: %d)", m_MemoryMap.size());
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: false)");
        return false;
    }

    switch (Type)
    {
    case ProgramAddressSpace_Is39Bit:
        m_AddressSpaceWidth = 39;
        m_AddressSpaceBase = 0x8000000;
        m_AddressSpaceSize = 0x7FF8000000;
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

    uint64_t AddressSpaceEnd = 1ULL << m_AddressSpaceWidth;
    MemoryRegion InitialMemRegion(0, AddressSpaceEnd, nullptr, MemoryState_None, MemoryType_Unmapped, MemoryAttr_None, MemoryPermission_Unmapped);
    WriteTrace(TraceMemory, TraceDebug, "Add base address space (Address: 0x%I64X Size: 0x%I64X)", InitialMemRegion.Address(), InitialMemRegion.Size());
    m_MemoryMap.insert(MemoryRegionMap::value_type(AddressSpaceEnd - 1, InitialMemRegion));
    WriteTrace(TraceMemory, TraceInfo, "Done (res: true)");
    return true;
}

bool CProcessMemory::SetHeapSize(uint64_t Size)
{
    if (Size > 0xc8000000 || m_heap != nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    m_heap = MapMemory(GetHeapRegionBaseAddr(), (uint32_t)Size, MemoryPermission_ReadWrite, MemoryType_Heap);
    return m_heap != nullptr;
}

bool CProcessMemory::GetMemoryInfo(uint64_t Address, QueryMemoryInfo & Info)
{
    MemoryRegionMap::const_iterator itr = m_MemoryMap.lower_bound(Address);
    if (itr != m_MemoryMap.end() && Address >= itr->second.Address() && (Address & ~3) + 4 < itr->first)
    {
        const MemoryRegion & Region = itr->second;
        Info.BaseAddress = Region.Address();
        Info.Size = Region.Size();
        Info.Type = Region.Type();
        Info.Permission = Region.Permission();
    }
    else
    {
        Info.BaseAddress = 1ULL << m_AddressSpaceWidth;
        Info.Size = 0 - Info.BaseAddress;
        Info.Type = MemoryType_Reserved;
        Info.Permission = MemoryPermission_None;
    }
    if (Info.Permission == MemoryPermission_Unmapped)
    {
        Info.Permission = MemoryPermission_None;
    }
    return true;
}

uint8_t * CProcessMemory::MapMemory(uint64_t Address, uint32_t Size, MemoryPermission Perm, MemoryType Type)
{
    WriteTrace(TraceMemory, TraceInfo, "Start (Address: 0x%I64X Size: 0x%X Perm: %s)", Address, Size, MemoryPermissionName(Perm), MemoryTypeName(Type));

    if (Size == 0)
    {
        WriteTrace(TraceMemory, TraceError, "Can not map, size == 0");
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: nullptr)");
        return nullptr;
    }

    if ((Address & CPageTable::PageMask) != 0)
    {
        WriteTrace(TraceMemory, TraceError, "Can not map, Address not page aligned (Address: 0x%I64X PageMask: 0x%X)", Address, CPageTable::PageMask);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: nullptr)");
        return nullptr;
    }

    if ((Size & CPageTable::PageMask) != 0)
    {
        WriteTrace(TraceMemory, TraceError, "Can not map, Size not page aligned (Size: 0x%X PageMask: 0x%X)", Size, CPageTable::PageMask);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: nullptr)");
        return nullptr;
    }

    MemoryRegionMapIter RegionItr;
    if (!CreateMemoryRegion(Address, Size, RegionItr))
    {
        WriteTrace(TraceMemory, TraceError, "Failed to create memory region (Address: 0x%I64X Size: 0x%X)", Address, Size);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: nullptr)");
        return nullptr;
    }
    MemoryRegion & Region = RegionItr->second;
    uint8_t * Memory = (uint8_t *)AllocateAddressSpace(Size);
    if (Memory == nullptr || CommitMemory(Memory, Size, MEM_READWRITE) == nullptr)
    {
        WriteTrace(TraceMemory, TraceError, "Failed to create memory (Size: 0x%X)", Size);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceInfo, "Done (res: nullptr)");
        return nullptr;
    }
    Region.m_Memory = Memory;
    Region.m_State = MemoryState_AllocatedMemory;
    Region.m_Type = Type;
    Region.m_Permission = Perm;
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
    void * ReadBuffer = nullptr;
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
    MemoryRegionMapIter itr;
    if (FindMemoryRegion(Addr, itr))
    {
        MemoryRegion & Region = itr->second;
        uint64_t StartIndex = Addr - Region.Address();
        for (uint64_t index = StartIndex, endIndex = itr->first - Region.Address(); index < endIndex; index++)
        {
            uint8_t * Memory = Region.Memory();
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

bool CProcessMemory::CreateMemoryRegion(uint64_t Address, uint64_t Size, MemoryRegionMapIter & Region)
{
    WriteTrace(TraceMemory, TraceVerbose, "Start (Address: 0x%I64X Size: 0x%X)", Address, Size);

    if ((Size & PageMask) != 0)
    {
        WriteTrace(TraceMemory, TraceError, "Can not create region, Size not page aligned (Size: 0x%X PageMask: 0x%X)", Size, PageMask);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }
    if ((Address & PageMask) != 0)
    {
        WriteTrace(TraceMemory, TraceError, "Can not create region, Address not page aligned (Address: 0x%I64X PageMask: 0x%X)", Address, PageMask);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }

    const uint64_t TargetEnd = Address + Size;
    if (TargetEnd < Address)
    {
        WriteTrace(TraceMemory, TraceError, "Address wraps past end (Address: 0x%I64X Size: 0x%X)", Address, Size);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }
    if (TargetEnd > (m_AddressSpaceBase + m_AddressSpaceSize))
    {
        WriteTrace(TraceMemory, TraceError, "Address passes address space end (Address: 0x%I64X Size: 0x%X AddressSpaceBase: 0x%I64X AddressSpaceSize: 0x%I64X)", Address, Size, m_AddressSpaceBase, m_AddressSpaceSize);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }
    if (Size == 0)
    {
        WriteTrace(TraceMemory, TraceError, "Size is 0 (Address: 0x%I64X Size: 0x%X)", Address, Size);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }

    MemoryRegionMapIter RegionStart;
    if (!FindMemoryRegion(Address, RegionStart))
    {
        WriteTrace(TraceMemory, TraceError, "Failed to find start region (Address: 0x%I64X)", Address, Size);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }

    WriteTrace(TraceMemory, TraceVerbose, "Found start region (Region Address: 0x%I64X Region Size: 0x%X)", RegionStart->second.Address(), RegionStart->second.Size());
    if (Address != RegionStart->second.Address())
    {
        WriteTrace(TraceMemory, TraceVerbose, "Start region needs to split");
        if (!SplitMemoryRegion(RegionStart, Address - RegionStart->second.Address()))
        {
            WriteTrace(TraceMemory, TraceError, "Failed to find split start region (Address: 0x%I64X Size: 0x%I64X)", Address, Size);
            g_Notify->BreakPoint(__FILE__, __LINE__);
            WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
            return false;
        }
    }

    MemoryRegionMapIter RegionEnd;
    if (!FindMemoryRegion(TargetEnd, RegionEnd))
    {
        WriteTrace(TraceMemory, TraceError, "Failed to find end region (Address: 0x%I64X)", Address, Size);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }

    if (TargetEnd != RegionEnd->second.Address())
    {
        WriteTrace(TraceMemory, TraceVerbose, "Region is to large, splitting end off (TargetEnd: 0x%I64X RegionEnd: 0x%I64X)", TargetEnd, RegionEnd->second.Address());
        if (!SplitMemoryRegion(RegionEnd, TargetEnd - RegionEnd->second.Address()))
        {
            WriteTrace(TraceMemory, TraceError, "Failed to find split end region (Address: 0x%I64X Size: 0x%I64X)", Address, Size);
            g_Notify->BreakPoint(__FILE__, __LINE__);
            WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
            return false;
        }
    }

    if (!FindMemoryRegion(Address, Region))
    {
        WriteTrace(TraceMemory, TraceError, "Failed to find new region (Address: 0x%I64X)", Address);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }

    if (Region->second.Address() != Address)
    {
        WriteTrace(TraceMemory, TraceError, "New region address does not match (Region Address: 0x%I64X Address: 0x%I64X)", Region->second.Address(), Address);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }
    if (Region->second.Size() != Size)
    {
        WriteTrace(TraceMemory, TraceError, "New region size does not match (Region Size: 0x%I64X Size: 0x%I64X)", Region->second.Size(), Size);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }
    WriteTrace(TraceMemory, TraceVerbose, "Done (res: True)");
    return true;
}

bool CProcessMemory::FindMemoryRegion(uint64_t Address, MemoryRegionMapIter & RegionItr)
{
    MemoryRegionMapIter itr = m_MemoryMap.lower_bound(Address);
    if (itr != m_MemoryMap.end() && Address >= itr->second.Address() && Address <= itr->first)
    {
        RegionItr = itr;
        return true;
    }
    return false;
}

bool CProcessMemory::FindAddressMemory(uint64_t Address, uint32_t len, void *& buffer)
{
    MemoryRegionMapIter itr;
    if (FindMemoryRegion(Address,itr))
    {
        MemoryRegion & Region = itr->second;
        if ((Region.Address() + Region.Size()) < Address + len)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        buffer = (void *)&(Region.Memory()[Address - Region.Address()]);
        return true;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool CProcessMemory::SplitMemoryRegion(MemoryRegionMapIter &SplitRegionItr, uint64_t Offset)
{
    WriteTrace(TraceMemory, TraceVerbose, "Start (SplitRegionItr Address: 0x%I64X Size: 0x%I64X Offset: 0x%I64X)", SplitRegionItr->second.Address(), SplitRegionItr->second.Size(), Offset);

    MemoryRegion OldRegion = SplitRegionItr->second;
    MemoryRegion NewRegion = OldRegion;

    if (Offset == 0)
    {
        WriteTrace(TraceMemory, TraceError, "Can not split region, Offset == 0");
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }
    if (Offset > OldRegion.Size())
    {
        WriteTrace(TraceMemory, TraceError, "Can not split region, Offset is larger then size, (Offset: 0x%I64X OldRegion.Size: 0x%I64X)", Offset, OldRegion.Size());
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }

    OldRegion.m_Size = Offset;
    NewRegion.m_Address += Offset;
    NewRegion.m_Size -= Offset;

    WriteTrace(TraceMemory, TraceVerbose, "Split memory state: %s", MemoryStateName(NewRegion.State()));
    switch (NewRegion.State())
    {
    case MemoryState_None:
        break;
    case MemoryState_AllocatedMemory:
    case MemoryState_UnmanagedMemory:
        NewRegion.m_State = MemoryState_UnmanagedMemory;
        NewRegion.m_Memory = &(OldRegion.Memory())[Offset];
        break;
    default:
        WriteTrace(TraceMemory, TraceError, "memory state not handled: %s", NewRegion.State());
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
        return false;
    }

    m_MemoryMap.erase(SplitRegionItr);
    m_MemoryMap.insert(MemoryRegionMap::value_type(OldRegion.Address() + OldRegion.Size() - 1, OldRegion));
    std::pair<MemoryRegionMap::iterator, bool> res = m_MemoryMap.insert(MemoryRegionMap::value_type(NewRegion.Address() + NewRegion.Size() - 1, NewRegion));
    if (res.second)
    {
        SplitRegionItr = res.first;
        WriteTrace(TraceMemory, TraceVerbose, "Done (res: true)");
        return true;
    }
    WriteTrace(TraceMemory, TraceError, "Failed to add memory map");
    g_Notify->BreakPoint(__FILE__, __LINE__);
    WriteTrace(TraceMemory, TraceVerbose, "Done (res: false)");
    return false;
}

