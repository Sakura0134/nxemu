#pragma once
#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\NamedPort.h>
#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\hle\Kernel\ResultCode.h>
#include <nxemu-core\hle\Kernel\KEvent.h>
#include <nxemu-core\hle\Kernel\Mutex.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <nxemu-core\hle\kernel\Service.h>
#include <Common\stdtypes.h>

class CSwitchSystem;

enum SvcCall
{
    SvcCall_SetHeapSize = 0x01,
    SvcCall_SetMemoryPermission = 0x02,
    SvcCall_SetMemoryAttribute = 0x03,
    SvcCall_MapMemory = 0x04,
    SvcCall_UnmapMemory = 0x05,
    SvcCall_QueryMemory = 0x06,
    SvcCall_ExitProcess = 0x07,
    SvcCall_CreateThread = 0x08,
    SvcCall_StartThread = 0x09,
    SvcCall_ExitThread = 0x0A,
    SvcCall_SleepThread = 0x0B,
    SvcCall_GetThreadPriority = 0x0C,
    SvcCall_SetThreadPriority = 0x0D,
    SvcCall_GetThreadCoreMask = 0x0E,
    SvcCall_SetThreadCoreMask = 0x0F,
    SvcCall_GetCurrentProcessorNumber = 0x10,
    SvcCall_SignalEvent = 0x11,
    SvcCall_ClearEvent = 0x12,
    SvcCall_MapSharedMemory = 0x13,
    SvcCall_UnmapSharedMemory = 0x14,
    SvcCall_CreateTransferMemory = 0x15,
    SvcCall_CloseHandle = 0x16,
    SvcCall_ResetSignal = 0x17,
    SvcCall_WaitSynchronization = 0x18,
    SvcCall_CancelSynchronization = 0x19,
    SvcCall_ArbitrateLock = 0x1A,
    SvcCall_ArbitrateUnlock = 0x1B,
    SvcCall_WaitProcessWideKeyAtomic = 0x1C,
    SvcCall_SignalProcessWideKey = 0x1D,
    SvcCall_GetSystemTick = 0x1E,
    SvcCall_ConnectToNamedPort = 0x1F,
    SvcCall_SendSyncRequestLight = 0x20,
    SvcCall_SendSyncRequest = 0x21,
    SvcCall_SendSyncRequestWithUserBuffer = 0x22,
    SvcCall_SendAsyncRequestWithUserBuffer = 0x23,
    SvcCall_GetProcessId = 0x24,
    SvcCall_GetThreadId = 0x25,
    SvcCall_Break = 0x26,
    SvcCall_OutputDebugString = 0x27,
    SvcCall_ReturnFromException = 0x28,
    SvcCall_GetInfo = 0x29,
    SvcCall_FlushEntireDataCache = 0x2A,
    SvcCall_FlushDataCache = 0x2B,
    SvcCall_MapPhysicalMemory = 0x2C,
    SvcCall_UnmapPhysicalMemory = 0x2D,
    SvcCall_GetDebugFutureThreadInfo = 0x2E,
    SvcCall_GetLastThreadInfo = 0x2F,
};

class CHleKernel
{
public:
    typedef std::map<std::string, CKernelObjectPtr> NamedPortList;

    enum
    {
        HANDLE_CURRENT_PROCESS = 0xFFFF8001
    };

    enum GetInfoType
    {
        AllowedCpuIdBitmask = 0,
        AllowedThreadPrioBitmask = 1,
        MapRegionBaseAddr = 2,
        MapRegionBaseSize = 3,
        HeapRegionBaseAddr = 4,
        HeapRegionSize = 5,
        TotalMemoryAvailable = 6,
        TotalHeapUsage = 7,
        IsCurrentProcessBeingDebugged = 8,
        RandomEntropy = 11,
        AddressSpaceBaseAddr = 12,
        AddressSpaceSize = 13,
        MapRegionAddress = 14,
        MapRegionSize = 15,
        unknown16 = 16,
    };

    enum
    {
        ConvertCurrentObjectToDomain = 0,
        CopyFromCurrentDomain = 1,
        CloneCurrentObject = 2,
        QueryPointerBufferSize = 3,
        CloneCurrentObjectEx = 4,
    };

    CHleKernel(CSwitchSystem & System, CProcessMemory & ProcessMemory);
    ~CHleKernel();

    bool AddSystemThread(uint32_t & ThreadHandle, const char * name, uint64_t entry_point, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId);
    uint32_t AddKernelObject(CKernelObject * object);
    KernelObjectMap KernelObjects(void);

    ResultCode CloseHandle(uint32_t Handle);
    ResultCode ConnectToNamedPort(CSystemThreadMemory & ThreadMemory, uint32_t & SessionHandle, uint64_t NameAddr);
    ResultCode CreateThread(uint32_t & ThreadHandle, uint64_t EntryPoint, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId);
    ResultCode GetInfo(uint64_t & Info, GetInfoType InfoType, uint32_t handle, uint64_t SubId);
    ResultCode GetThreadPriority(uint32_t & Priority, uint32_t handle);
    ResultCode MapMemory(uint64_t DstAddress, uint64_t SrcAddress, uint64_t Size);
    ResultCode QueryMemory(CSystemThreadMemory & ThreadMemory, uint64_t MemoryInfoAddr, uint64_t QueryAddr);
    ResultCode SendSyncRequest(uint32_t Handle);
    ResultCode SetHeapSize(uint64_t & HeapAddress, uint64_t size);
    ResultCode SignalProcessWideKey(uint64_t ptr, uint32_t value);
    ResultCode StartThread(uint32_t ThreadHandle);
    ResultCode WaitProcessWideKeyAtomic(uint64_t ptr0, uint64_t ptr1, uint32_t ThreadHandle, uint64_t timeout);
    ResultCode WaitSynchronization(CSystemThreadMemory & ThreadMemory, uint32_t & HandleIndex, uint64_t HandlesPtr, uint32_t HandlesNum, uint64_t Timeout);

    static const char * GetSvcCallStr(SvcCall Call);

private:
    CHleKernel(void);                          // Disable default constructor
    CHleKernel(const CHleKernel&);             // Disable copy constructor
    CHleKernel& operator=(const CHleKernel&);  // Disable assignment

    ResultCode ProcessSyncRequest(CService * Service, CIPCRequest & Request);
    ResultCode ProcessSyncControl(CService * Service, CIPCRequest & Request);
    uint32_t GetNewHandle();
    uint32_t CreateNewThreadID();
    static const char * GetInfoTypeName(GetInfoType Id);

    CriticalSection m_CS;
    CSwitchSystem & m_System;
    CProcessMemory & m_ProcessMemory;
    KernelObjectMap m_KernelObjects;
    CMutex m_Mutex;
    CServiceManger m_SM;
    NamedPortList m_NamedPorts;
    uint32_t m_NextHandle;
    uint32_t m_NextThreadId;
    uint64_t m_RandomEntropy[4];
};