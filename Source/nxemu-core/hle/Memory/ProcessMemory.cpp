#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\MemoryManagement.h>

const uint64_t CProcessMemory::PageBits = 12;
const uint64_t CProcessMemory::PageSize = 1 << PageBits;
const uint64_t CProcessMemory::PageMask = PageSize - 1;

CProcessMemory::CProcessMemory(CSwitchSystem & System) :
    m_System(System),
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
    CMemoryRegion InitialMemRegion(0, AddressSpaceEnd, nullptr, MemoryState_None, MemoryType_Unmapped, MemoryAttr_None, MemoryPermission_Unmapped);
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

bool CProcessMemory::SetMemoryAttribute(uint64_t Addr, uint64_t Size, MemoryAttribute Mask, MemoryAttribute Attribute)
{
    MemoryRegionMapIter RegionItr;
    if (!CreateMemoryRegion(Addr, Size, RegionItr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }
    CMemoryRegion& Region = RegionItr->second;
    Region.m_Attribute = (MemoryAttribute)((Region.Attribute() & ~Mask) | (Mask & Attribute));
    return true;
}

bool CProcessMemory::ReprotectRange(uint64_t Target, uint64_t Size, MemoryPermission NewPerm)
{
    MemoryRegionMapIter RegionItr;
    if (!CreateMemoryRegion(Target, Size, RegionItr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }
    uint64_t TargetEnd = Target + Size;
    while (RegionItr != m_MemoryMap.end())
    {
        CMemoryRegion & Region = RegionItr->second;
        Region.m_Permission = NewPerm;
        RegionItr++;
        if (RegionItr->second.Address() == TargetEnd)
        {
            break;
        }
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return true;
}

bool CProcessMemory::GetMemoryInfo(uint64_t Address, QueryMemoryInfo & Info)
{
    MemoryRegionMap::const_iterator itr = m_MemoryMap.lower_bound(Address);
    if (itr != m_MemoryMap.end() && Address >= itr->second.Address() && (Address & ~3) + 4 < itr->first)
    {
        const CMemoryRegion & Region = itr->second;
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
    CMemoryRegion & Region = RegionItr->second;
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

bool CProcessMemory::MapMemoryBlock(uint64_t Address, std::shared_ptr<std::vector<uint8_t>> Block, uint64_t Offset, uint64_t Size, MemoryType MemType)
{
    MemoryRegionMapIter RegionItr;
    if (!CreateMemoryRegion(Address, Size, RegionItr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }

    CMemoryRegion & Region = RegionItr->second;
    Region.m_State = MemoryState_BackingMemory;
    Region.m_Type = MemType;
    Region.m_Permission = MemoryPermission_ReadWrite;
    Region.m_BackingBlock = Block;
    Region.m_BackingOffset = Offset;
    return true;
}

bool CProcessMemory::MirrorMemory(uint64_t DstAddress, uint64_t SrcAddress, uint64_t Size)
{
    MemoryRegionMapIter OrigDstRegionItr;
    if (!FindMemoryRegion(DstAddress, OrigDstRegionItr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    if (OrigDstRegionItr->second.State() != MemoryState_None || OrigDstRegionItr->second.Type() != MemoryAttr_None)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    MemoryRegionMapIter OrigSrcRegionItr;
    if (!FindMemoryRegion(SrcAddress, OrigSrcRegionItr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    if (OrigSrcRegionItr->second.State() != MemoryState_AllocatedMemory || 
        (OrigSrcRegionItr->second.Type() != MemoryType_Heap && OrigSrcRegionItr->second.Type() != MemoryType_CodeMutable))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    MemoryRegionMapIter SrcRegionItr;
    if (!CreateMemoryRegion(SrcAddress, Size, SrcRegionItr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }

    MemoryRegionMapIter DstRegionItr;
    if (!CreateMemoryRegion(DstAddress, Size, DstRegionItr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }

    CMemoryRegion & SrcRegion = SrcRegionItr->second;
    CMemoryRegion & DstRegion = DstRegionItr->second;
    DstRegion.m_Attribute = SrcRegion.m_Attribute;
    DstRegion.m_Type = SrcRegion.m_Type;
    DstRegion.m_Permission = SrcRegion.m_Permission;
    DstRegion.m_State = MemoryState_UnmanagedMemory;
    DstRegion.m_Memory = SrcRegion.m_Memory;
    DstRegion.m_Permission = MemoryPermission_ReadWrite;
    SrcRegion.m_Permission = MemoryPermission_None;
    return true;
}

bool CProcessMemory::Read32(uint64_t Addr, uint32_t & value) const
{
    if ((Addr & 3) != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); }

    return ReadBytes(Addr, (uint8_t *)&value, sizeof(value), false);
}

bool CProcessMemory::Read64(uint64_t Addr, uint64_t & value) const
{
    if ((Addr & 7) != 0) { g_Notify->BreakPoint(__FILE__, __LINE__); }

    return ReadBytes(Addr, (uint8_t *)&value, sizeof(value), false);
}

bool CProcessMemory::ReadBytes(uint64_t Address, uint8_t * buffer, uint32_t len, bool external) const
{
    CGuard Guard(m_CS);

    MemoryRegionMapIter itr;
    if (!FindMemoryRegion(Address, itr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    CMemoryRegion& Region = itr->second;
    if ((Region.Address() + Region.Size()) < Address + len)
    {
        uint64_t Regionlen = Region.Size() - (Region.Address() - Address);
        if (!ReadBytes(Address + Regionlen, buffer + Regionlen, len - (uint32_t)Regionlen, external))
        {
            return false;
        }
        len = (uint32_t)Regionlen;
    }

    if (!external && Region.State() == MemoryState_RasterizerMemory)
    {
        m_System.Video().FlushRegion(Address, len);
    }
    if (Region.Memory() == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    void* ReadBuffer = (void*)&(Region.Memory()[Address - Region.Address()]);
    memcpy(buffer, ReadBuffer, len);
    return true;
}

bool CProcessMemory::ReadCString(uint64_t Addr, std::string & value) const
{
    CGuard Guard(m_CS);

    MemoryRegionMapIter itr;
    if (FindMemoryRegion(Addr, itr))
    {
        CMemoryRegion & Region = itr->second;
        uint64_t StartIndex = Addr - Region.Address();
        for (uint64_t index = StartIndex, endIndex = itr->first - Region.Address(); index < endIndex; index++)
        {
            uint8_t * Memory = Region.Memory();
            if (Memory[index] != 0)
            {
                continue;
            }
            if (Region.State() == MemoryState_RasterizerMemory)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            value = std::string((const char *)&Memory[StartIndex], index - StartIndex);
            return true;
        }
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

bool CProcessMemory::WriteBytes(uint64_t Address, const uint8_t* buffer, uint32_t len, bool external)
{
    CGuard Guard(m_CS);

    MemoryRegionMapIter itr;
    if (!FindMemoryRegion(Address, itr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    CMemoryRegion& Region = itr->second;
    if ((Region.Address() + Region.Size()) < Address + len)
    {
        uint64_t Regionlen = Region.Size() - (Region.Address() - Address);
        if (!WriteBytes(Address + Regionlen, buffer + Regionlen, len - (uint32_t)Regionlen, external))
        {
            return false;
        }
        len = (uint32_t)Regionlen;
    }

    if (!external && Region.State() == MemoryState_RasterizerMemory)
    {
        m_System.Video().InvalidateRegion(Address, len);
    }
    if (Region.Memory() == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    void * WriteBuffer = (void*)&(Region.Memory()[Address - Region.Address()]);
    memcpy(WriteBuffer, buffer, len);
    return true;
}

void CProcessMemory::MarkRasterizerMemory(uint64_t Addr, uint64_t Size)
{
    CGuard Guard(m_CS);

    MemoryRegionMapIter RegionItr;
    if (!CreateMemoryRegion(Addr, Size, RegionItr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    CMemoryRegion & Region = RegionItr->second;
    if (Region.m_Type != MemoryType_Heap && Region.m_State != MemoryState_UnmanagedMemory)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    Region.m_State = MemoryState_RasterizerMemory;
    Region.m_Attribute = MemoryAttr_None;
}

uint8_t* CProcessMemory::GetPointer(uint64_t Address)
{
    CGuard Guard(m_CS);

    MemoryRegionMapIter itr;
    if (FindMemoryRegion(Address, itr))
    {
        CMemoryRegion& Region = itr->second;
        if (Region.Memory() == nullptr)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        return (uint8_t*)&(Region.Memory()[Address - Region.Address()]);
    }
    return nullptr;
}

const uint8_t * CProcessMemory::GetPointer(uint64_t Address) const
{
    MemoryRegionMap::const_iterator itr = m_MemoryMap.lower_bound(Address);
    if (itr != m_MemoryMap.end() && Address >= itr->second.Address() && Address <= itr->first)
    {
        const CMemoryRegion & Region = itr->second;
        if (Region.Memory() == nullptr)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return nullptr;
        }
        return (const uint8_t*)&(Region.Memory()[Address - Region.Address()]);
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

bool CProcessMemory::CreateMemoryRegion(uint64_t Address, uint64_t Size, MemoryRegionMapIter & Region)
{
    CGuard Guard(m_CS);

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

bool CProcessMemory::FindMemoryRegion(uint64_t Address, MemoryRegionMap::const_iterator& RegionItr) const
{
    MemoryRegionMap::const_iterator itr = m_MemoryMap.lower_bound(Address);
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
        CMemoryRegion & Region = itr->second;
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

    CMemoryRegion OldRegion = SplitRegionItr->second;
    CMemoryRegion NewRegion = OldRegion;

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

