#include <nxemu-core\hle\Memory\MemoryTypes.h>
#include <Common\StdString.h>

const char * ProgramAddressSpaceTypeName(ProgramAddressSpaceType Type)
{
    switch (Type)
    {
    case ProgramAddressSpace_Is32Bit: return "ProgramAddressSpace_Is32Bit";
    case ProgramAddressSpace_Is36Bit: return "ProgramAddressSpace_Is36Bit";
    case ProgramAddressSpace_Is32BitNoMap: return "ProgramAddressSpace_Is32BitNoMap";
    case ProgramAddressSpace_Is39Bit: return "ProgramAddressSpace_Is39Bit";
    }

    static stdstr unknown;
    unknown.Format("Unknown (0x%X)", Type);
    return unknown.c_str();
}

const char * MemoryPermissionName(MemoryPermission Permission)
{
    switch (Permission)
    {
    case MemoryPermission_Reserved: return "MemoryPermission_Reserved";
    case MemoryPermission_Invalid: return "MemoryPermission_Invalid";
    case MemoryPermission_Unmapped: return "MemoryPermission_Unmapped";
    case MemoryPermission_None: return "MemoryPermission_None";
    case MemoryPermission_Read: return "MemoryPermission_Read";
    case MemoryPermission_Write: return "MemoryPermission_Write";
    case MemoryPermission_Execute: return "MemoryPermission_Execute";
    case MemoryPermission_ReadWrite: return "MemoryPermission_ReadWrite";
    case MemoryPermission_ReadExecute: return "MemoryPermission_ReadExecute";
    case MemoryPermission_ReadWriteExecute: return "MemoryPermission_ReadWriteExecute";
    case MemoryPermission_DontCare: return "MemoryPermission_DontCare";
    };

    static stdstr unknown;
    unknown.Format("Unknown (0x%X)", Permission);
    return unknown.c_str();
}

const char * MemoryTypeName(MemoryType Type)
{
    switch (Type)
    {
    case MemoryType_Unmapped: return "MemoryType_Unmapped";
    case MemoryType_Io: return "MemoryType_Io";
    case MemoryType_Normal: return "MemoryType_Normal";
    case MemoryType_CodeStatic: return "MemoryType_CodeStatic";
    case MemoryType_CodeMutable: return "MemoryType_CodeMutable";
    case MemoryType_Heap: return "MemoryType_Heap";
    case MemoryType_SharedMemory: return "MemoryType_SharedMemory";
    case MemoryType_WeirdMappedMemory: return "MemoryType_WeirdMappedMemory";
    case MemoryType_ModuleCodeStatic: return "MemoryType_ModuleCodeStatic";
    case MemoryType_ModuleCodeMutable: return "MemoryType_ModuleCodeMutable";
    case MemoryType_IpcBuffer0: return "MemoryType_IpcBuffer0";
    case MemoryType_MappedMemory: return "MemoryType_MappedMemory";
    case MemoryType_ThreadLocal: return "MemoryType_ThreadLocal";
    case MemoryType_TransferMemoryIsolated: return "MemoryType_TransferMemoryIsolated";
    case MemoryType_TransferMemory: return "MemoryType_TransferMemory";
    case MemoryType_ProcessMemory: return "MemoryType_ProcessMemory";
    case MemoryType_Reserved: return "MemoryType_Reserved";
    case MemoryType_IpcBuffer1: return "MemoryType_IpcBuffer1";
    case MemoryType_IpcBuffer3: return "MemoryType_IpcBuffer3";
    case MemoryType_KernelStack: return "MemoryType_KernelStack";
    case MemoryType_CodeReadOnly: return "MemoryType_CodeReadOnly";
    case MemoryType_CodeWritable: return "MemoryType_CodeWritable";
    }

    static stdstr unknown;
    unknown.Format("Unknown (0x%X)", Type);
    return unknown.c_str();
}
