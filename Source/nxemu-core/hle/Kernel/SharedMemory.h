#pragma once
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <nxemu-core\hle\Memory\MemoryTypes.h>
#include <memory>

class CHleKernel;

class CKernelSharedMemory :
    public CKernelObject
{
public:
    static CKernelObjectPtr Create(uint64_t Size, MemoryPermission Permissions, MemoryPermission OtherPermissions, uint64_t Address = 0, const char * Name = "Unknown");

private:
    KernelObjectHandleType GetHandleType() const { return KernelObjectHandleType_SharedMemory; }
    CKernelSharedMemory * GetSharedMemoryPtr(void) { return this; }

    CKernelSharedMemory(void);
    CKernelSharedMemory(const CKernelSharedMemory&);
    CKernelSharedMemory& operator=(const CKernelSharedMemory&);

    MemoryPermission m_Permissions;
    MemoryPermission m_OtherPermissions;
    uint64_t m_Address;
    uint64_t m_Size;
    std::string m_Name;
    std::shared_ptr<std::vector<uint8_t>> m_BackingBlock;
    uint64_t m_BackingBlockOffset;
};

