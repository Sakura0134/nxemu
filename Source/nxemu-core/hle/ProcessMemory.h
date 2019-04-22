#pragma once
#include <Common\stdtypes.h>
#include <map>
#include <nxemu-core\Machine\PageTable.h>

class CSystemThreadMemory;

class CProcessMemory
{
    friend CSystemThreadMemory;

public:
    typedef CPageTable::MemoryPermission MemoryPermission;

    enum MemoryType
    {
        MemoryType_Unmapped = 0,
        MemoryType_Io = 1,
        MemoryType_Normal = 2,
        MemoryType_CodeStatic = 3,
        MemoryType_CodeMutable = 4,
        MemoryType_Heap = 5,
        MemoryType_SharedMemory = 6,
        MemoryType_WeirdMappedMemory = 7,
        MemoryType_ModuleCodeStatic = 8,
        MemoryType_ModuleCodeMutable = 9,
        MemoryType_IpcBuffer0 = 10,
        MemoryType_MappedMemory = 11,
        MemoryType_ThreadLocal = 12,
        MemoryType_TransferMemoryIsolated = 13,
        MemoryType_TransferMemory = 14,
        MemoryType_ProcessMemory = 15,
        MemoryType_Reserved = 16,
        MemoryType_IpcBuffer1 = 17,
        MemoryType_IpcBuffer3 = 18,
        MemoryType_KernelStack = 19,
        MemoryType_CodeReadOnly = 20,
        MemoryType_CodeWritable = 21,
    };

    static uint64_t GetAddressSpaceBaseAddr(void) { return 0x0000000008000000; }

    CProcessMemory();
    ~CProcessMemory();
        
    uint8_t * MapMemory(uint64_t Address, uint32_t Size, MemoryPermission Perm, MemoryType type);
    bool Read32(uint64_t Addr, uint32_t & value);
    bool Read64(uint64_t Addr, uint64_t & value);
    bool ReadBytes(uint64_t Addr, uint8_t * buffer, uint32_t len);
    bool ReadCString(uint64_t Addr, std::string & value);

private:
    CProcessMemory(const CProcessMemory&);            // Disable copy constructor
    CProcessMemory& operator=(const CProcessMemory&); // Disable assignment

    bool FindAddressMemory(uint64_t Addr, uint32_t len, void *& buffer);

    typedef struct
    {
        uint64_t start_addr;
        uint8_t * memory;
        MemoryType type;
    } MemoryInfo;

    typedef std::map<uint64_t, MemoryInfo> MemoryMap;

    MemoryMap m_MemoryMap;
};