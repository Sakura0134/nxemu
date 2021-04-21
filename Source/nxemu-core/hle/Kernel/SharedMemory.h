#pragma once

#include <nxemu-core\hle\Kernel\KernelObject.h>

class CHleKernel;

class CKernelSharedMemory :
    public CKernelObject
{
private:
    KernelObjectHandleType GetHandleType() const { return KernelObjectHandleType_SharedMemory; }
    CKernelSharedMemory * GetSharedMemoryPtr(void) { return this; }

    CKernelSharedMemory(void);
    CKernelSharedMemory(const CKernelSharedMemory&);
    CKernelSharedMemory& operator=(const CKernelSharedMemory&);
};

