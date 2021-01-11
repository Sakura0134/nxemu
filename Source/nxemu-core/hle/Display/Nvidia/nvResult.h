#pragma once

enum nvResult
{
    nvResult_Success = 0,
    nvResult_NotImplemented = 1,
    nvResult_NotSupported = 2,
    nvResult_NotInitialized = 3,
    nvResult_BadParameter = 4,
    nvResult_Timeout = 5,
    nvResult_InsufficientMemory = 6,
    nvResult_ReadOnlyAttribute = 7,
    nvResult_InvalidState = 8,
    nvResult_InvalidAddress = 9,
    nvResult_InvalidSize = 0xA,
    nvResult_BadValue = 0xB,
    nvResult_AlreadyAllocated = 0xD,
    nvResult_Busy = 0xE,
    nvResult_ResourceError = 0xF,
    nvResult_CountMismatch = 0x10,
    nvResult_SharedMemoryTooSmall = 0x1000,
    nvResult_FileOperationFailed = 0x30003,
    nvResult_DirOperationFailed = 0x30004,
    nvResult_IoctlFailed = 0x3000F,
    nvResult_AccessDenied = 0x30010,
    nvResult_FileNotFound = 0x30013,
    nvResult_ModuleNotPresent = 0xA000E,
};