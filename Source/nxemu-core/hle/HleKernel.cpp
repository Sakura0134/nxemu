#include <nxemu-core\hle\HleKernel.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\StdString.h>

CHleKernel::CHleKernel(CSwitchSystem & System, CProcessMemory & ProcessMemory) :
    m_System(System),
    m_ProcessMemory(ProcessMemory),
    m_NextHandle(0x8002),
	m_NextThreadId(1)
{
}

CHleKernel::~CHleKernel()
{
    for (SystemThreadList::iterator itr = m_SystemThreads.begin(); itr != m_SystemThreads.end(); itr++)
    {
        delete itr->second;
    }
    m_SystemThreads.clear();
}

bool CHleKernel::AddSystemThread(uint32_t & ThreadHandle, const char * name, uint64_t entry_point, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId)
{
	ThreadHandle = GetNewHandle();
	CSystemThread * Thread = new CSystemThread(this, m_ProcessMemory, name, entry_point, ThreadHandle, CreateNewThreadID(), ThreadContext, StackTop, StackSize, Priority, ProcessorId);
    if (Thread == NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    Thread->Reg().Set64(Arm64Opcode::ARM64_REG_X1, ThreadHandle);
	m_SystemThreads.insert(SystemThreadList::value_type(ThreadHandle, Thread));
    return true;
}

uint32_t CHleKernel::GetNewHandle()
{
	uint32_t handle = m_NextHandle++;
	return handle;
}

uint64_t CHleKernel::CreateNewThreadID()
{
	uint64_t ThreadId = m_NextThreadId++;
	return ThreadId;
}

const char * CHleKernel::SvcGetCallStr(SvcGetCall svcCall)
{
    switch (svcCall)
    {
    case svcSetHeapSize: return "SetHeapSize";
    case svcSetMemoryPermission: return "SetMemoryPermission";
    case svcSetMemoryAttribute: return "SetMemoryAttribute";
    case svcMapMemory: return "MapMemory";
    case svcUnmapMemory: return "UnmapMemory";
    case svcQueryMemory: return "QueryMemory";
    case svcExitProcess: return "ExitProcess";
    case svcCreateThread: return "CreateThread";
    case svcStartThread: return "StartThread";
    case svcExitThread: return "ExitThread";
    case svcSleepThread: return "SleepThread";
    case svcGetThreadPriority: return "GetThreadPriority";
    case svcSetThreadPriority: return "SetThreadPriority";
    case svcGetThreadCoreMask: return "GetThreadCoreMask";
    case svcSetThreadCoreMask: return "SetThreadCoreMask";
    case svcGetCurrentProcessorNumber: return "GetCurrentProcessorNumber";
    case svcSignalEvent: return "SignalEvent";
    case svcClearEvent: return "ClearEvent";
    case svcMapSharedMemory: return "MapSharedMemory";
    case svcUnmapSharedMemory: return "UnmapSharedMemory";
    case svcCreateTransferMemory: return "CreateTransferMemory";
    case svcCloseHandle: return "CloseHandle";
    case svcResetSignal: return "ResetSignal";
    case svcWaitSynchronization: return "WaitSynchronization";
    case svcCancelSynchronization: return "CancelSynchronization";
    case svcArbitrateLock: return "ArbitrateLock";
    case svcArbitrateUnlock: return "ArbitrateUnlock";
    case svcWaitProcessWideKeyAtomic: return "WaitProcessWideKeyAtomic";
    case svcSignalProcessWideKey: return "SignalProcessWideKey";
    case svcGetSystemTick: return "GetSystemTick";
    case svcConnectToNamedPort: return "ConnectToNamedPort";
    case svcSendSyncRequestLight: return "SendSyncRequestLight";
    case svcSendSyncRequest: return "SendSyncRequest";
    case svcSendSyncRequestWithUserBuffer: return "SendSyncRequestWithUserBuffer";
    case svcSendAsyncRequestWithUserBuffer: return "SendAsyncRequestWithUserBuffer";
    case svcGetProcessId: return "GetProcessId";
    case svcGetThreadId: return "GetThreadId";
    case svcBreak: return "Break";
    case svcOutputDebugString: return "OutputDebugString";
    case svcReturnFromException: return "ReturnFromException";
    case svcGetInfo: return "GetInfo";
    case svcFlushEntireDataCache: return "FlushEntireDataCache";
    case scvFlushDataCache: return "FlushDataCache";
    case svcMapPhysicalMemory: return "MapPhysicalMemory";
    case svcUnmapPhysicalMemory: return "UnmapPhysicalMemory";
    case svcGetDebugFutureThreadInfo: return "GetDebugFutureThreadInfo";
    case svcGetLastThreadInfo: return "GetLastThreadInfo";
    }
    static stdstr unknown;
    unknown.Format("Unknown (0x%X)", svcCall);
    return unknown.c_str();
}
