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
    m_PageTable(PageTableSize) 
{
}

CVideoMemory::~CVideoMemory()
{
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
    Align = Align == 0 ? Align::Up(Align, PageSize) : PageSize;

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

