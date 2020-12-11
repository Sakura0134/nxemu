#include <nxemu-core\hle\Memory\MemoryRegion.h>

CMemoryRegion::CMemoryRegion() :
    m_Address(0),
    m_Size(0),
    m_Memory(nullptr),
    m_State(MemoryState_None),
    m_Type(MemoryType_Unmapped),
    m_Attribute(MemoryAttr_None),
    m_Permission(MemoryPermission_None)
{
}

CMemoryRegion::CMemoryRegion(uint64_t Addr, uint64_t Size, uint8_t * Memory, MemoryState State, MemoryType Type, MemoryAttribute Attribute, MemoryPermission Permission) :
    m_Address(Addr),
    m_Size(Size),
    m_Memory(Memory),
    m_State(State),
    m_Type(Type),
    m_Attribute(Attribute),
    m_Permission(Permission)
{
}

CMemoryRegion::CMemoryRegion(const CMemoryRegion&rhs) :
    m_Address(rhs.m_Address),
    m_Size(rhs.m_Size),
    m_Memory(rhs.m_Memory),
    m_State(rhs.m_State),
    m_Type(rhs.m_Type),
    m_Attribute(rhs.m_Attribute),
    m_Permission(rhs.m_Permission)
{
}

CMemoryRegion & CMemoryRegion::operator=(const CMemoryRegion & rhs)
{
    m_Address = rhs.m_Address;
    m_Size = rhs.m_Size;
    m_Memory = rhs.m_Memory;
    m_State = rhs.m_State;
    m_Type = rhs.m_Type;
    m_Attribute = rhs.m_Attribute;
    m_Permission = rhs.m_Permission;
    return *this;
}