#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\Machine\PageTable.h>
#include <nxemu-core\hle\Memory\MemoryTypes.h>
#include <map>

class CSystemThreadMemory;

class CProcessMemory
{
    friend CSystemThreadMemory;

public:
    CProcessMemory();
    ~CProcessMemory();
    

    static uint64_t GetAddressSpaceBaseAddr(void) { return 0x0000000008000000; }
    uint64_t GetTlsIoRegionBase(void) const { return m_TlsIoRegionBase; }
    uint64_t GetTlsIoRegionSize(void) const { return m_TlsIoRegionSize; }

    bool Initialize(ProgramAddressSpaceType Type, bool Is64bit);
        
    uint8_t * MapMemory(uint64_t Address, uint32_t Size, MemoryPermission Perm, MemoryType type);
    bool Read32(uint64_t Addr, uint32_t & value);
    bool Read64(uint64_t Addr, uint64_t & value);
    bool ReadBytes(uint64_t Addr, uint8_t * buffer, uint32_t len);
    bool ReadCString(uint64_t Addr, std::string & value);

private:
    CProcessMemory(const CProcessMemory&);
    CProcessMemory& operator=(const CProcessMemory&);

    bool FindAddressMemory(uint64_t Addr, uint32_t len, void *& buffer);

    typedef struct
    {
        uint64_t start_addr;
        uint8_t * memory;
        MemoryType type;
    } MemoryInfo;

    typedef std::map<uint64_t, MemoryInfo> MemoryMap;

    uint64_t m_CodeRegionStart, m_CodeRegionSize;
    uint64_t m_MapRegionBase, m_MapRegionSize;
    uint64_t m_HeapRegionBase, m_HeapRegionSize;
    uint64_t m_NewMapRegionBase, m_NewMapRegionSize;
    uint64_t m_TlsIoRegionBase, m_TlsIoRegionSize;
    MemoryMap m_MemoryMap;
};