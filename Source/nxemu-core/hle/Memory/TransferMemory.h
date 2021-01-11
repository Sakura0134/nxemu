#pragma once 
#include <Common\stdtypes.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>

class CTransferMemory :
    public CKernelObject
{
public:
    CTransferMemory(uint64_t Addr, uint64_t Size, uint32_t Permissions);

private:
    CTransferMemory(void);
    CTransferMemory(const CTransferMemory&);
    CTransferMemory& operator=(const CTransferMemory&);

    KernelObjectHandleType GetHandleType() const { return KernelObjectHandleType_Event; }
    CTransferMemory * GetTransferMemoryPtr(void) { return this; }

    uint64_t m_Addr;
    uint64_t m_Size;
    uint32_t m_Permissions;
};
