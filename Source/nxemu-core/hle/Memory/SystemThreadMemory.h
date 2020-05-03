#pragma once
#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\Machine\CPU\MemoryManagement.h>
#include <nxemu-core\hle\Memory\ProcessMemory.h>

class CSystemThreadMemory :
    public MemoryManagement
{
public:
    CSystemThreadMemory(CProcessMemory &ProcessMemory, CPUExecutor * Executor);
    ~CSystemThreadMemory();

    bool Initialize(uint64_t StackTop, uint32_t StackSize, uint64_t TlsAddress, uint32_t TlsSize);
    bool Read8(uint64_t Addr, uint8_t & value);
	bool Read32(uint64_t Addr, uint32_t & value);
	bool Read64(uint64_t Addr, uint64_t & value);
    bool ReadBytes(uint64_t Addr, uint8_t * buffer, uint32_t len);
    bool Write8(uint64_t Addr, uint8_t value);
    bool Write16(uint64_t Addr, uint16_t value);
    bool Write32(uint64_t Addr, uint32_t value);
    bool Write64(uint64_t Addr, uint64_t value);
    bool WriteBytes(uint64_t Addr, const uint8_t * buffer, uint32_t len);
    bool GetMemoryInfo(uint64_t Addr, QueryMemoryInfo & MemoryInfo);

private:
    CSystemThreadMemory();                                      // Disable default constructor
    CSystemThreadMemory(const CSystemThreadMemory&);            // Disable copy constructor
    CSystemThreadMemory& operator=(const CSystemThreadMemory&); // Disable assignment

	bool FindAddressMemory(uint64_t Addr, uint32_t len, void *& buffer);

    CProcessMemory & m_ProcessMemory;
    uint8_t * m_stackmem;
    uint64_t m_StackAddress;
    uint32_t m_StackSize;
    uint64_t m_tlsAddress;
    uint32_t m_tlsSize;
    uint8_t * m_tls;
    CPUExecutor * m_Executor;
};