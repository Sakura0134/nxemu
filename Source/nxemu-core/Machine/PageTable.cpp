#include <nxemu-core\Machine\PageTable.h>

uint64_t CPageTable::PageRoundDown(uint64_t Value)
{
    return Value & ~PageMask;
}

uint64_t CPageTable::PageRoundUp(uint64_t Value)
{
    return PageRoundDown(Value + PageMask);
}

