#pragma once
#include <nxemu-core\hle\Memory\MemoryTypes.h>
#include <stdint.h>
#include <map>
#include <memory>
#include <vector>

class CProcessMemory;

class CMemoryRegion
{
    friend CProcessMemory;

public:
    CMemoryRegion();
    CMemoryRegion(uint64_t Addr, uint64_t Size, uint8_t * Memory, MemoryState State, MemoryType Type, MemoryAttribute Attribute, MemoryPermission Permission);
    CMemoryRegion(const CMemoryRegion&);
    CMemoryRegion& operator=(const CMemoryRegion&);

    inline uint64_t Address(void) const { return m_Address; };
    inline uint64_t Size(void) const { return m_Size; };
    inline uint8_t * Memory(void) const { return m_Memory; };
    inline MemoryState State(void) const { return m_State; };
    inline MemoryType Type(void) const { return m_Type; };
    inline MemoryAttribute Attribute(void) const { return m_Attribute; }
    inline MemoryPermission Permission(void) const { return m_Permission; };
    inline uint8_t * BackingMemory(void) const { return m_BackingBlock.get() != nullptr ? &(*m_BackingBlock.get())[0] : nullptr; };

private:
    uint64_t m_Address;
    uint64_t m_Size;
    uint8_t * m_Memory;
    MemoryState m_State;
    MemoryType m_Type;
    MemoryAttribute m_Attribute;
    MemoryPermission m_Permission;
    std::shared_ptr<std::vector<uint8_t>> m_BackingBlock;
    uint64_t m_BackingOffset;
};

typedef std::map<uint64_t, CMemoryRegion> MemoryRegionMap;
typedef MemoryRegionMap::iterator MemoryRegionMapIter;
