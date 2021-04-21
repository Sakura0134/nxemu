#pragma once
#include <nxemu-core\hle\Kernel\SharedMemory.h>
#include <memory>

class CInterfaceDevice
{
public:
    enum
    {
        SHARED_MEMORY_SIZE = 0x40000
    };

    CInterfaceDevice(CHleKernel& Kernel);
    ~CInterfaceDevice();

    CKernelObjectPtr GetSharedMemory();

private:
    CKernelObjectPtr m_SharedMem;
    CHleKernel& m_Kernel;
};