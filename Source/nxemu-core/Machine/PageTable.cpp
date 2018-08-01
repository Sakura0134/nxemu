#include <nxemu-core\Machine\PageTable.h>

uint64_t PageTable::PageRoundDown(uint64_t Value)
{
    return Value & ~PageMask;
}

uint64_t PageTable::PageRoundUp(uint64_t Value)
{
    return PageRoundDown(Value + PageMask);
}

