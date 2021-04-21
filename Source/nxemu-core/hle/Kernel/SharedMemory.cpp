#include <nxemu-core\hle\Kernel\SharedMemory.h>
#include <SystemGlobals.h>

CKernelSharedMemory::CKernelSharedMemory() :
    m_Permissions(MemoryPermission_None),
    m_OtherPermissions(MemoryPermission_None),
    m_Address(0),
    m_Size(0),
    m_BackingBlockOffset(0)
{
}

CKernelObjectPtr CKernelSharedMemory::Create(uint64_t Size, MemoryPermission Permissions, MemoryPermission OtherPermissions, uint64_t Address, const char * Name)
{
    CKernelObjectPtr KernelObject(new CKernelSharedMemory());
    CKernelSharedMemory * memory = KernelObject->GetSharedMemoryPtr();

    memory->m_Permissions = Permissions;
    memory->m_OtherPermissions = OtherPermissions;
    memory->m_Address = Address;
    memory->m_Size = Size;
    memory->m_Name = Name != nullptr ? Name : "";

    if (Address == 0) 
    {
        memory->m_BackingBlock = std::make_shared<std::vector<uint8_t>>(Size);
        memory->m_BackingBlockOffset = 0;

    } 
    else 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return memory;
}