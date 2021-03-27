#pragma once
#include <stdint.h>
#include <string>

__interface MemoryManagement
{
	virtual bool Read8(uint64_t Addr, uint8_t & value) = 0;
	virtual bool Read16(uint64_t Addr, uint16_t & value) = 0;
	virtual bool Read32(uint64_t Addr, uint32_t & value) = 0;
    virtual bool Read64(uint64_t Addr, uint64_t & value) = 0;
    virtual bool Write8(uint64_t Addr, uint8_t value) = 0;
    virtual bool Write16(uint64_t Addr, uint16_t value) = 0;
    virtual bool Write32(uint64_t Addr, uint32_t value) = 0;
    virtual bool Write64(uint64_t Addr, uint64_t value) = 0;
    virtual bool WriteBytes(uint64_t Addr, const uint8_t * buffer, uint32_t len) = 0;
};