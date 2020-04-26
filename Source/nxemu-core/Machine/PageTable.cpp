#include <nxemu-core\Machine\PageTable.h>

uint64_t CPageTable::PageRoundDown(uint64_t Value)
{
    return Value & ~PageMask;
}

uint64_t CPageTable::PageRoundUp(uint64_t Value)
{
    return PageRoundDown(Value + PageMask);
}

const char * CPageTable::MemoryPermissionName(MemoryPermission Perm)
{
    switch (Perm)
    {
    case MemoryPermission_Invalid: return "Invalid";
    case MemoryPermission_Unmapped: return "Unmapped";
    case MemoryPermission_Read: return "Read";
    case MemoryPermission_Write: return "Write";
    case MemoryPermission_Execute: return "Execute";
    case MemoryPermission_ReadWrite: return "ReadWrite";
    case MemoryPermission_ReadExecute: return "ReadExecute";
    }
    return "Unknown";
}
