#pragma once
#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\NamedPort.h>
#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\hle\Kernel\ResultCode.h>
#include <nxemu-core\hle\Kernel\KEvent.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <nxemu-core\hle\kernel\Service.h>
#include <Common\stdtypes.h>

class CSwitchSystem;

class CHleKernel
{
public:
    typedef std::map<std::string, CKernelObjectPtr> NamedPortList;

    enum
    {
        HANDLE_CURRENT_PROCESS = 0xFFFF8001
    };

    enum SvcGetCall
    {
        svcSetHeapSize = 0x01,
        svcSetMemoryPermission = 0x02,
        svcSetMemoryAttribute = 0x03,
        svcMapMemory = 0x04,
        svcUnmapMemory = 0x05,
        svcQueryMemory = 0x06,
        svcExitProcess = 0x07,
        svcCreateThread = 0x08,
        svcStartThread = 0x09,
        svcExitThread = 0x0A,
        svcSleepThread = 0x0B,
        svcGetThreadPriority = 0x0C,
        svcSetThreadPriority = 0x0D,
        svcGetThreadCoreMask = 0x0E,
        svcSetThreadCoreMask = 0x0F,
        svcGetCurrentProcessorNumber = 0x10,
        svcSignalEvent = 0x11,
        svcClearEvent = 0x12,
        svcMapSharedMemory = 0x13,
        svcUnmapSharedMemory = 0x14,
        svcCreateTransferMemory = 0x15,
        svcCloseHandle = 0x16,
        svcResetSignal = 0x17,
        svcWaitSynchronization = 0x18,
        svcCancelSynchronization = 0x19,
        svcArbitrateLock = 0x1A,
        svcArbitrateUnlock = 0x1B,
        svcWaitProcessWideKeyAtomic = 0x1C,
        svcSignalProcessWideKey = 0x1D,
        svcGetSystemTick = 0x1E,
        svcConnectToNamedPort = 0x1F,
        svcSendSyncRequestLight = 0x20,
        svcSendSyncRequest = 0x21,
        svcSendSyncRequestWithUserBuffer = 0x22,
        svcSendAsyncRequestWithUserBuffer = 0x23,
        svcGetProcessId = 0x24,
        svcGetThreadId = 0x25,
        svcBreak = 0x26,
        svcOutputDebugString = 0x27,
        svcReturnFromException = 0x28,
        svcGetInfo = 0x29,
        svcFlushEntireDataCache = 0x2A,
        scvFlushDataCache = 0x2B,
        svcMapPhysicalMemory = 0x2C,
        svcUnmapPhysicalMemory = 0x2D,
        svcGetDebugFutureThreadInfo = 0x2E,
        svcGetLastThreadInfo = 0x2F,
    };

    enum GetInfoType : uint64_t
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
    KernelObjectList & ThreadQueue(void) { return m_ThreadQueue; }
    KernelObjectMap & KernelObjects(void) { return m_KernelObjects; }

    uint32_t AddKernelObject(CKernelObject * object);

    ResultCode CloseHandle(uint32_t Handle);
    ResultCode ConnectToNamedPort(CSystemThreadMemory & ThreadMemory, uint32_t & SessionHandle, uint64_t NameAddr);
    ResultCode GetInfo(uint64_t & Info, GetInfoType InfoType, uint32_t handle, uint64_t SubId);
    ResultCode GetThreadPriority(uint32_t & Priority, uint32_t handle);
    ResultCode QueryMemory(CSystemThreadMemory & ThreadMemory, uint64_t MemoryInfoAddr, uint64_t QueryAddr);
    ResultCode SendSyncRequest(uint32_t Handle);
    ResultCode SetHeapSize(uint64_t & HeapAddress, uint64_t size);
    ResultCode SignalProcessWideKey(uint64_t ptr, uint32_t value);
    ResultCode WaitSynchronization(CSystemThreadMemory & ThreadMemory, uint32_t & HandleIndex, uint64_t HandlesPtr, uint32_t HandlesNum, uint64_t Timeout);

    static const char * SvcGetCallStr(SvcGetCall svcCall);

private:
    CHleKernel(void);                          // Disable default constructor
    CHleKernel(const CHleKernel&);             // Disable copy constructor
    CHleKernel& operator=(const CHleKernel&);  // Disable assignment

    ResultCode ProcessSyncRequest(CService * Service, CIPCRequest & Request);
    ResultCode ProcessSyncControl(CService * Service, CIPCRequest & Request);
    uint32_t GetNewHandle();
    uint32_t CreateNewThreadID();
    static const char * GetInfoTypeName(GetInfoType Id);

    CSwitchSystem & m_System;
    CProcessMemory & m_ProcessMemory;
    KernelObjectMap m_KernelObjects;
    KernelObjectList m_ThreadQueue;
    CServiceManger m_SM;
    NamedPortList m_NamedPorts;
    uint32_t m_NextHandle;
    uint32_t m_NextThreadId;
    uint64_t m_RandomEntropy[4];
};