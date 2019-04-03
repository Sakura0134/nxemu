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
    case Invalid: return "Invalid";
    case Unmapped: return "Unmapped";
    case Read: return "Read";
    case Write: return "Write";
    case Execute: return "Execute";
    case ReadWrite: return "ReadWrite";
    case ReadExecute: return "ReadExecute";
    }
    return "Unknown";
}
