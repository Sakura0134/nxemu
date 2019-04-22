#pragma once
#include <Common\stdtypes.h>
#include <string>

__interface MemoryManagement
{
	virtual bool Read32(uint64_t Addr, uint32_t & value) = 0;
};