#pragma once
#include <nxemu-core\hle\Memory\MemoryTypes.h>
#include <Common\stdtypes.h>
#include <map>
#include <memory>
#include <vector>

class CProcessMemory;

class MemoryRegion
{
    friend CProcessMemory;

public:
    MemoryRegion();
    MemoryRegion(uint64_t Addr, uint64_t Size, uint8_t * Memory, MemoryState State, MemoryType Type, MemoryAttribute Attribute, MemoryPermission Permission);
    MemoryRegion(const MemoryRegion&);
    MemoryRegion& operator=(const MemoryRegion&);

    inline uint64_t Address(void) const { return m_Address; };
    inline uint64_t Size(void) const { return m_Size; };
    inline uint8_t * Memory(void) const { return m_Memory; };
    inline MemoryState State(void) const { return m_State; };
    inline MemoryType Type(void) const { return m_Type; };
    inline MemoryAttribute Attribute(void) const { return m_Attribute; }
    inline MemoryPermission Permission(void) const { return m_Permission; };

private:
    uint64_t m_Address;
    uint64_t m_Size;
    uint8_t * m_Memory;
    MemoryState m_State;
    MemoryType m_Type;
    MemoryAttribute m_Attribute;
    MemoryPermission m_Permission;
};

typedef std::map<uint64_t, MemoryRegion> MemoryRegionMap;
typedef MemoryRegionMap::iterator MemoryRegionMapIter;
