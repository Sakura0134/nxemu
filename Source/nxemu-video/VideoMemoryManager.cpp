#include "VideoMemoryManager.h"
#include "VideoNotification.h"
#include <Common\Align.h>

CVideoMemory::PageEntry::PageEntry() :
    m_State((uint32_t)State_Unmapped)
{
}

CVideoMemory::PageEntry::PageEntry(State State) :
    m_State(State)
{
}

CVideoMemory::PageEntry::PageEntry(uint64_t Addr) :
    m_State((uint32_t)(Addr >> ShiftBits))
{
}

bool CVideoMemory::PageEntry::IsUnmapped() const
{
    return m_State == State_Unmapped;
}

bool CVideoMemory::PageEntry::IsAllocated() const
{
    return m_State == State_Allocated;
}

bool CVideoMemory::PageEntry::IsValid() const
{
    return !IsUnmapped() && !IsAllocated();
}

CVideoMemory::PageEntry CVideoMemory::PageEntry::operator+(uint64_t offset) const
{
    if (!IsValid())
    {
        return *this;
    }
    return PageEntry((((uint64_t)m_State) << ShiftBits) + offset);
}

CVideoMemory::CVideoMemory(ISwitchSystem & System) : 
    m_System(System), 
    m_PageTable(PageTableSize),
    m_Renderer(nullptr) 
{
}

CVideoMemory::~CVideoMemory()
{
}

void CVideoMemory::BindRenderer(IRenderer * Renderer) 
{
    m_Renderer = Renderer;
}

void CVideoMemory::UpdateRange(uint64_t GpuAddr, PageEntry PageEntry, uint64_t Size)
{
    for (uint64_t Offset = 0; Offset < Size; Offset += PageSize) 
    {
        SetPageEntry(GpuAddr + Offset, PageEntry + Offset);
    }
}

void CVideoMemory::Map(uint64_t CpuAddr, uint64_t GpuAddr, uint64_t Size)
{
    UpdateRange(GpuAddr, CpuAddr, Size);
}

uint64_t CVideoMemory::MapAllocate(uint64_t CpuAddr, uint64_t Size, uint64_t Align) 
{
    uint64_t GpuAddr = 0;
    if (!FindFreeRange(GpuAddr, Size, Align))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    UpdateRange(GpuAddr, CpuAddr, Size);
    return GpuAddr;
}

bool CVideoMemory::AllocateFixed(uint64_t GpuAddr, uint64_t Size) 
{
    for (uint64_t Offset = 0; Offset < Size; Offset += PageSize) 
    {
        if (!GetPageEntry(GpuAddr + Offset).IsUnmapped())
        {
            return false;
        }
    }

    UpdateRange(GpuAddr, PageEntry::State_Allocated, Size);
    return true;
}

uint64_t CVideoMemory::Allocate(uint64_t Size, uint64_t Align)
{
    uint64_t GpuAddr = 0;
    if (!FindFreeRange(GpuAddr, Size, Align))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    if (!AllocateFixed(GpuAddr, Size)) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;    
    }
    return GpuAddr;
}

CVideoMemory::PageEntry CVideoMemory::GetPageEntry(uint64_t gpu_addr) const
{
    return m_PageTable[PageEntryIndex(gpu_addr)];
}

void CVideoMemory::SetPageEntry(uint64_t GpuAddr, PageEntry PageEntry) 
{
    m_PageTable[PageEntryIndex(GpuAddr)] = PageEntry;
}

bool CVideoMemory::FindFreeRange(uint64_t & GpuAddr, uint64_t Size, uint64_t Align) const
{
    uint64_t AvailableSize = 0, TestAddr = AddressSpaceStart;
    Align = Align == 0 ? AlignUp(Align, PageSize) : PageSize;

    while (TestAddr + AvailableSize < AddressSpaceSize) 
    {
        if (GetPageEntry(TestAddr + AvailableSize).IsUnmapped()) 
        {
            AvailableSize += PageSize;

            if (AvailableSize >= Size) 
            {
                GpuAddr = TestAddr;
                return true;
            }
        } 
        else
        {
            TestAddr += AvailableSize + PageSize;
            AvailableSize = 0;

            uint64_t remainder = TestAddr % Align;
            if (remainder != 0)
            {
                TestAddr = (TestAddr - remainder) + Align;
            }
        }
    }
    return false;
}

uint64_t CVideoMemory::PageEntryIndex(uint64_t gpu_addr)
{
    return (gpu_addr >> PageBits) & PageTableMask;
}

bool CVideoMemory::GpuToCpuAddress(uint64_t GpuAddr, uint64_t& CpuAddress) const
{
    const PageEntry & Entry = m_PageTable[PageEntryIndex(GpuAddr)];
    if (!Entry.IsValid()) 
    {
        return false;
    }
    CpuAddress = Entry.Address() + (GpuAddr & PageMask);
    return true;
}

uint8_t * CVideoMemory::GetPointer(uint64_t GpuAddr)
{
    if (!GetPageEntry(GpuAddr).IsValid()) 
    {
        return nullptr;
    }

    uint64_t CpuAddr;
    if (!GpuToCpuAddress(GpuAddr, CpuAddr)) 
    {
        return nullptr;
    }

    return m_System.GetCPUMemoryPointer(CpuAddr);
}

void CVideoMemory::ReadBuffer(uint64_t GpuAddr, void * Buffer, uint64_t Size) const 
{
    uint64_t RemainingSize = Size;
    uint64_t PageIndex = GpuAddr >> PageBits;
    uint64_t PageOffset = GpuAddr & PageMask;

    while (RemainingSize > 0) 
    {
        uint64_t CopyAmount = PageSize - PageOffset;
        if (RemainingSize < CopyAmount) 
        {
            CopyAmount = RemainingSize;
        }

        uint64_t CpuAddr;
        if (GpuToCpuAddress(PageIndex << PageBits, CpuAddr))
        {
            if (!m_System.ReadCPUMemory(CpuAddr + PageOffset, Buffer, CopyAmount)) 
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        } 
        else 
        {
            std::memset(Buffer, 0, CopyAmount);
        }

        PageIndex += 1;
        PageOffset = 0;
        Buffer = ((uint8_t *)Buffer) + CopyAmount;
        RemainingSize -= CopyAmount;
    }
}

void CVideoMemory::WriteBuffer(uint64_t GpuAddr, const void* Buffer, uint64_t Size, bool InvalidateRegion)
{
    uint64_t RemainingSize = Size;
    uint64_t PageIndex = GpuAddr >> PageBits;
    uint64_t PageOffset = GpuAddr & PageMask;

    while (RemainingSize > 0) 
    {
        uint64_t CopyAmount = PageSize - PageOffset;
        if (RemainingSize < CopyAmount) 
        {
            CopyAmount = RemainingSize;
        }

        uint64_t CpuAddr;
        if (GpuToCpuAddress(PageIndex << PageBits, CpuAddr))
        {
            uint64_t DestAddr = CpuAddr + PageOffset;
            if (InvalidateRegion)
            {
                m_Renderer->InvalidateRegion(DestAddr, CopyAmount);            
            }
            if (!m_System.WriteCPUMemory(DestAddr, Buffer, CopyAmount))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        } 
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        PageIndex += 1;
        PageOffset = 0;
        Buffer = ((uint8_t*)Buffer) + CopyAmount;
        RemainingSize -= CopyAmount;
    }
}

bool CVideoMemory::IsGranularRange(uint64_t GpuAddr, uint64_t Size) const
{
    uint64_t CpuAddr;
    if (!GpuToCpuAddress(GpuAddr, CpuAddr))
    {
        return false;
    }
    uint64_t Page = (CpuAddr & CpuPageMask) + Size;
    return Page <= CpuPageSize;
}
