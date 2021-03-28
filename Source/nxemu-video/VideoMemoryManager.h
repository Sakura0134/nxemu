#pragma once
#include <nxemu-plugin-spec\Video.h>
#include <vector>

class CVideoMemory 
{
    enum : uint64_t
    {
        AddressSpaceSize = 1ULL << 40,
        AddressSpaceStart = 1ULL << 32,
        PageBits = 16,
        PageSize = 1 << PageBits,
        PageTableBits = 24,
        PageTableSize = 1 << PageTableBits,
        PageTableMask = PageTableSize - 1,
    };

    class PageEntry
    {
        enum 
        { 
            ShiftBits = 12,
        };

    public:
        enum State 
        {
            State_Unmapped = (uint32_t)-1,
            State_Allocated = (uint32_t)-2,
        };

        PageEntry();
        PageEntry(State State);
        PageEntry(uint64_t Addr);

        bool IsUnmapped() const;
        bool IsAllocated() const;
        bool IsValid() const;
        PageEntry operator+(uint64_t offset) const;

    private:
        uint32_t m_State;
    };

public:
    CVideoMemory(ISwitchSystem & System);
    ~CVideoMemory();

    bool AllocateFixed(uint64_t GpuAddr, uint64_t Size);
    uint64_t Allocate(uint64_t Size, uint64_t Align);

private:
    CVideoMemory();
    CVideoMemory(const CVideoMemory&);
    CVideoMemory & operator=(const CVideoMemory &);

    PageEntry GetPageEntry(uint64_t gpu_addr) const;
    void SetPageEntry(uint64_t GpuAddr, PageEntry PageEntry);
    void UpdateRange(uint64_t GpuAddr, PageEntry PageEntry, uint64_t Size);
    bool FindFreeRange(uint64_t & GpuAddr, uint64_t Size, uint64_t Align) const;
    static uint64_t PageEntryIndex(uint64_t gpu_addr);

    ISwitchSystem & m_System;
    std::vector<PageEntry> m_PageTable;
};
