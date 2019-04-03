#pragma once
#include <Common\stdtypes.h>

class CPageTable
{
public:
    enum
    {
        PageBits = 12,
        PageSize = 1 << PageBits,
        PageMask = PageSize - 1,
        Lvl1Bit = PageBits,
    };

    enum MemoryPermission
    {
        Reserved = -2,
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
    CPageTable(void);
    CPageTable(const CPageTable&);             // Disable copy constructor
    CPageTable& operator=(const CPageTable&);  // Disable assignment
};
