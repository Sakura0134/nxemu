#pragma once
#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\Machine\CPU\MemoryManagement.h>
#include <nxemu-core\hle\ProcessMemory.h>

class CSystemThreadMemory :
    public MemoryManagement
{
	typedef CProcessMemory::MemoryMap MemoryMap;

public:
    CSystemThreadMemory(CProcessMemory &ProcessMemory, CPUExecutor * Executor);
    ~CSystemThreadMemory();

	bool Read32(uint64_t Addr, uint32_t & value);
    bool ReadBytes(uint64_t Addr, uint8_t * buffer, uint32_t len);
    bool Write8(uint64_t Addr, uint8_t value);
    bool Write16(uint64_t Addr, uint16_t value);
    bool Write32(uint64_t Addr, uint32_t value);
    bool Write64(uint64_t Addr, uint64_t value);
    bool WriteBytes(uint64_t Addr, const uint8_t * buffer, uint32_t len);

private:
    CSystemThreadMemory();                                      // Disable default constructor
    CSystemThreadMemory(const CSystemThreadMemory&);            // Disable copy constructor
    CSystemThreadMemory& operator=(const CSystemThreadMemory&); // Disable assignment

	bool FindAddressMemory(uint64_t Addr, uint32_t len, void *& buffer);

    CProcessMemory & m_ProcessMemory;
    CPUExecutor * m_Executor;
};