#pragma once
#include <nxemu-core\Machine\PageTable.h>
#include <nxemu-core\hle\Memory\MemoryRegion.h>
#include <Common\CriticalSection.h>
#include <stdint.h>
#include <map>

class CSystemThreadMemory;

class CProcessMemory
{
    friend CSystemThreadMemory;

public:
    CProcessMemory();
    ~CProcessMemory();

    uint64_t GetAddressSpaceBaseAddr(void) const { return m_AddressSpaceBase; }
    uint64_t GetAddressSpaceSize(void) const { return m_AddressSpaceSize; }
    uint64_t GetHeapRegionBaseAddr(void) const { return m_HeapRegionBase; }
    uint64_t GetMapRegionAddress(void) const { return m_NewMapRegionBase; }
    uint64_t GetMapRegionSize(void) const { return m_NewMapRegionSize; }
    uint64_t GetTlsIoRegionBase(void) const { return m_TlsIoRegionBase; }
    uint64_t GetTlsIoRegionSize(void) const { return m_TlsIoRegionSize; }

    bool Initialize(ProgramAddressSpaceType Type, bool Is64bit);
    bool SetHeapSize(uint64_t Size);
    bool SetMemoryAttribute(uint64_t Addr, uint64_t Size, MemoryAttribute Mask, MemoryAttribute Attribute);
    bool ReprotectRange(uint64_t Target, uint64_t Size, MemoryPermission NewPerm);

    bool GetMemoryInfo(uint64_t Address, QueryMemoryInfo & Info);
    uint8_t * MapMemory(uint64_t Address, uint32_t Size, MemoryPermission Perm, MemoryType Type);
    bool MapMemoryBlock(uint64_t Address, std::shared_ptr<std::vector<uint8_t>> Block, uint64_t Offset, uint64_t size, MemoryType MemType);
	bool MirrorMemory(uint64_t DstAddress, uint64_t Address, uint64_t Size);
    bool Read32(uint64_t Addr, uint32_t & value) const;
    bool Read64(uint64_t Addr, uint64_t & value) const;
    bool ReadBytes(uint64_t Addr, uint8_t * buffer, uint32_t len, bool external) const;
    bool ReadCString(uint64_t Addr, std::string & value) const;
    bool WriteBytes(uint64_t Addr, const uint8_t * Buffer, uint32_t Len, bool External);
    uint8_t* GetPointer(uint64_t Address);
    const uint8_t* GetPointer(uint64_t Address) const;

private:
    CProcessMemory(const CProcessMemory&);
    CProcessMemory& operator=(const CProcessMemory&);

    bool CreateMemoryRegion(uint64_t Address, uint64_t Size, MemoryRegionMapIter & RegionItr);
    bool FindMemoryRegion(uint64_t Address, MemoryRegionMap::const_iterator& RegionItr) const;
    bool FindAddressMemory(uint64_t Address, uint32_t len, void *& buffer);
    bool SplitMemoryRegion(MemoryRegionMapIter & SplitRegionItr, uint64_t Offset);

    static const uint64_t PageBits, PageSize, PageMask;

    uint32_t m_AddressSpaceWidth;
    uint64_t m_AddressSpaceBase, m_AddressSpaceSize;
    uint64_t m_CodeRegionStart, m_CodeRegionSize;
    uint64_t m_MapRegionBase, m_MapRegionSize;
    uint64_t m_HeapRegionBase, m_HeapRegionSize;
    uint64_t m_NewMapRegionBase, m_NewMapRegionSize;
    uint64_t m_TlsIoRegionBase, m_TlsIoRegionSize;

    mutable CriticalSection m_CS;
    MemoryRegionMap m_MemoryMap;
    uint8_t * m_heap;
};