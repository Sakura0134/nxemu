#pragma once
#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\hle\Kernel\SystemThread.h>

class CSwitchSystem;

class CHleKernel
{
public:   
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

    CHleKernel(CSwitchSystem & System, CProcessMemory & ProcessMemory);
    ~CHleKernel();

	bool AddSystemThread(uint32_t & ThreadHandle, const char * name, uint64_t entry_point, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId);
    const SystemThreadList & SystemThreads(void) const { return m_SystemThreads; }

    static const char * SvcGetCallStr(SvcGetCall svcCall);

private:
    CHleKernel(void);                          // Disable default constructor
    CHleKernel(const CHleKernel&);             // Disable copy constructor
    CHleKernel& operator=(const CHleKernel&);  // Disable assignment

	uint32_t GetNewHandle();
	uint64_t CreateNewThreadID();

    CSwitchSystem & m_System;
    CProcessMemory & m_ProcessMemory;
    SystemThreadList m_SystemThreads;
    uint32_t m_NextHandle;
	uint64_t m_NextThreadId;
};