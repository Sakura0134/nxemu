#pragma once
#include <Common\stdtypes.h>

enum ProgramAddressSpaceType
{
    ProgramAddressSpace_Is32Bit = 0,
    ProgramAddressSpace_Is36Bit = 1,
    ProgramAddressSpace_Is32BitNoMap = 2,
    ProgramAddressSpace_Is39Bit = 3,
};

enum MemoryPermission
{
    MemoryPermission_Reserved = -3,
    MemoryPermission_Invalid = -2,
    MemoryPermission_Unmapped = -1,
    MemoryPermission_None = 0,
    MemoryPermission_Read = 1,
    MemoryPermission_Write = 2,
    MemoryPermission_Execute = 4,
    MemoryPermission_ReadWrite = MemoryPermission_Read | MemoryPermission_Write,
    MemoryPermission_ReadExecute = MemoryPermission_Read | MemoryPermission_Execute,
    MemoryPermission_ReadWriteExecute = MemoryPermission_Read | MemoryPermission_Write | MemoryPermission_Execute,
    MemoryPermission_DontCare = (1u << 28)
};

enum MemoryType
{
    MemoryType_Unmapped = 0,
    MemoryType_Io = 1,
    MemoryType_Normal = 2,
    MemoryType_CodeStatic = 3,
    MemoryType_CodeMutable = 4,
    MemoryType_Heap = 5,
    MemoryType_SharedMemory = 6,
    MemoryType_WeirdMappedMemory = 7,
    MemoryType_ModuleCodeStatic = 8,
    MemoryType_ModuleCodeMutable = 9,
    MemoryType_IpcBuffer0 = 10,
    MemoryType_MappedMemory = 11,
    MemoryType_ThreadLocal = 12,
    MemoryType_TransferMemoryIsolated = 13,
    MemoryType_TransferMemory = 14,
    MemoryType_ProcessMemory = 15,
    MemoryType_Reserved = 16,
    MemoryType_IpcBuffer1 = 17,
    MemoryType_IpcBuffer3 = 18,
    MemoryType_KernelStack = 19,
    MemoryType_CodeReadOnly = 20,
    MemoryType_CodeWritable = 21,
};

enum MemoryAttribute
{
    MemoryAttr_None = 0,
    MemoryAttr_Uncached = 8,
};

enum MemoryState
{
    MemoryState_None,
    MemoryState_AllocatedMemory,
    MemoryState_UnmanagedMemory,
};

typedef struct
{
    uint64_t BaseAddress;
    uint64_t Size;
    MemoryType Type;
    MemoryAttribute Attributes;
    MemoryPermission Permission;
    uint32_t DeviceRefCount;
    uint32_t IpcRefCount;
    uint32_t Reserved;
} QueryMemoryInfo;

const char * ProgramAddressSpaceTypeName(ProgramAddressSpaceType Type);
const char * MemoryPermissionName(MemoryPermission Permission);
const char * MemoryTypeName(MemoryType Type);
const char * MemoryStateName(MemoryState State);
