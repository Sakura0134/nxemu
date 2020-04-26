#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\hle\Memory\MemoryTypes.h>

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

    static uint64_t PageRoundDown(uint64_t Value);
    static uint64_t PageRoundUp(uint64_t Value);

    static const char * MemoryPermissionName(MemoryPermission Perm);
private:
    CPageTable(void);
    CPageTable(const CPageTable&);             // Disable copy constructor
    CPageTable& operator=(const CPageTable&);  // Disable assignment
};
