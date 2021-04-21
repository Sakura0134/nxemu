#include <nxemu-core\hle\HID\InterfaceDevice.h>
#include <nxemu-core\SystemGlobals.h>

CInterfaceDevice::CInterfaceDevice(CHleKernel& Kernel) :
    m_Kernel(Kernel),
    m_SharedMem(nullptr)
{
}

CInterfaceDevice::~CInterfaceDevice()
{
}

CKernelObjectPtr CInterfaceDevice::GetSharedMemory()
{
    if (m_SharedMem.get() == nullptr)
    {
        m_SharedMem = CKernelSharedMemory::Create(SHARED_MEMORY_SIZE, MemoryPermission_ReadWrite, MemoryPermission_Read, 0, "HID:SharedMemory");
        if (m_SharedMem.get() == nullptr)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    return m_SharedMem;
}
