#pragma once
#include <Common\stdtypes.h>

class PageTable
{
public:
    enum
    {
        PageBits = 12,
        PageSize = 1 << PageBits,
        PageMask = PageSize - 1,
        Lvl1Bits = 10,
        Lvl1Size = 1 << Lvl1Bits,
        Lvl1Mask = Lvl1Size - 1,
        Lvl1Bit = PageBits,
        Lvl0Bits = 10,
        Lvl0Size = 1 << Lvl0Bits,
        Lvl0Mask = Lvl0Size - 1,
        Lvl0Bit = PageBits + Lvl1Bits,
    };

    enum MemoryPermission
    {
        Invalid = -1,
        Unmapped = 0,
        Read = 1,
        Write = 2,
        Execute = 4,
        ReadWrite = Read | Write,
        ReadExecute = Read | Execute
    };

    static uint64_t PageRoundDown(uint64_t Value);
    static uint64_t PageRoundUp(uint64_t Value);

private:
    PageTable(const PageTable&);				// Disable copy constructor
    PageTable& operator=(const PageTable&);		// Disable assignment
};
