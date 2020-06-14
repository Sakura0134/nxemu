#include <nxemu-core\hle\Kernel\HleKernel.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\StdString.h>
#include <Common\Random.h>
#include <ctime>
#include <algorithm>

CHleKernel::CHleKernel(CSwitchSystem & System, CProcessMemory & ProcessMemory) :
    m_System(System),
    m_ProcessMemory(ProcessMemory),
    m_SM(System),
    m_NextHandle(0x8002),
    m_NextThreadId(1)
{
    CRandom EntropyRandomize(g_Settings->LoadBool(HleKernel_RandomizeEntropy) ? (uint32_t)time(NULL) : g_Settings->LoadDword(HleKernel_RandomizeSeed));
    for (uint32_t i = 0, n = sizeof(m_RandomEntropy) / sizeof(m_RandomEntropy[0]); i < n; i++)
    {
        m_RandomEntropy[i] = ((uint64_t)EntropyRandomize.next() << 32) | EntropyRandomize.next();
    }

    m_NamedPorts.insert(NamedPortList::value_type("sm:", &m_SM));
}

CHleKernel::~CHleKernel()
{
}

uint32_t CHleKernel::AddKernelObject(CKernelObject * Object)
{
    if (Object == NULL)
    {
        return (uint32_t)-1;
    }
    uint32_t Handle = GetNewHandle();
    m_KernelObjects.insert(KernelObjectMap::value_type(Handle, Object));
    return Handle;
}

ResultCode CHleKernel::CloseHandle(uint32_t Handle)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (Handle: 0x%X)", Handle);

    KernelObjectMap::iterator itr = m_KernelObjects.find(Handle);
    if (itr != m_KernelObjects.end())
    {
        m_KernelObjects.erase(itr);
    }
    else
    {
        WriteTrace(TraceHleKernel, TraceError, "Failed to find handle 0x%X", Handle);
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    WriteTrace(TraceHleKernel, TraceInfo, "Done (result: 0x%X)", 0);
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::ConnectToNamedPort(CSystemThreadMemory & ThreadMemory, uint32_t & result, uint64_t NameAddr)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (NameAddr: 0x%I64X)", NameAddr);

    std::string Name;
    if (!ThreadMemory.ReadCString(NameAddr, Name))
    {
        WriteTrace(TraceHleKernel, TraceInfo, "Done - Failed to read name (Return: 0x%I64X)", 0xE601);
        return ERR_NOT_FOUND;
    }

    if (Name.length() > 11)
    {
        WriteTrace(TraceHleKernel, TraceInfo, "Done - Name to long (Return: 0x%I64X)", 0xEE01);
        return ERR_OUT_OF_RANGE;
    }

    WriteTrace(TraceHleKernel, TraceInfo, "Name: \"%s\"", Name.c_str());
    NamedPortList::iterator itr = m_NamedPorts.find(Name);
    if (itr == m_NamedPorts.end())
    {
        WriteTrace(TraceHleKernel, TraceInfo, "Done - Failed to find handler (Return: 0x%I64X)", 0xF201);
        return ERR_NOT_FOUND;
    }
    CService * NamedPort = itr->second->GetServicePtr();
    if (!NamedPort->Connect())
    {
        WriteTrace(TraceHleKernel, TraceInfo, "Done - Connect failed (Return: 0x%I64X)", 0x10801);
        return ERR_NOT_FOUND;
    }

    result = AddKernelObject(NamedPort);
    WriteTrace(TraceHleKernel, TraceInfo, "Done (handle: 0x%I64X Return: 0x%I64X)", result, 0);
    return 0;
}

ResultCode CHleKernel::GetInfo(uint64_t & Info, GetInfoType InfoType, uint32_t handle, uint64_t SubId)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (InfoType: %s handle: 0x%X SubId: 0x%I64X)", GetInfoTypeName(InfoType), handle, SubId);
    ResultCode Result = RESULT_SUCCESS;

    if (InfoType == RandomEntropy)
    {
        if (handle != 0)
        {
            WriteTrace(TraceHleKernel, TraceNotice, "InfoType: %s: Process handle is not 0 (%X)", GetInfoTypeName(InfoType), handle);
            Result = ERR_INVALID_HANDLE;
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        else if (SubId >= sizeof(m_RandomEntropy) / sizeof(m_RandomEntropy[0]))
        {
            WriteTrace(TraceHleKernel, TraceNotice, "InfoType: %s: SubId to high (%X) - Max (%X)", GetInfoTypeName(InfoType), SubId, sizeof(m_RandomEntropy) / sizeof(m_RandomEntropy[0]));
            Result = ERR_INVALID_COMBINATION;
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        else
        {
            Info = m_RandomEntropy[SubId];
        }
    }
    else if (InfoType == MapRegionAddress || InfoType == MapRegionSize || InfoType == AddressSpaceBaseAddr || InfoType == AddressSpaceSize)
    {
        if (SubId != 0)
        {
            WriteTrace(TraceHleKernel, TraceNotice, "InfoType: %s: SubId should be 0, was (%X)", GetInfoTypeName(InfoType), SubId);
            Result = ERR_INVALID_ENUM_VALUE;
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        else if (handle != HANDLE_CURRENT_PROCESS)
        {
            WriteTrace(TraceHleKernel, TraceNotice, "handle (%X) not handled", handle);
            Result = ERR_INVALID_HANDLE;
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        else
        {
            switch (InfoType)
            {
            case AddressSpaceBaseAddr:
                Info = m_ProcessMemory.GetAddressSpaceBaseAddr();
                break;
            case AddressSpaceSize:
                Info = m_ProcessMemory.GetAddressSpaceSize();
                break;
            case MapRegionAddress:
                Info = m_ProcessMemory.GetMapRegionAddress();
                break;
            case MapRegionSize:
                Info = m_ProcessMemory.GetMapRegionSize();
                break;
            default:
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    WriteTrace(TraceHleKernel, TraceInfo, "Done (Info: 0x%I64X Return: %s)", Info, ResultCodeStr(Result));
    return Result;
}

ResultCode CHleKernel::GetThreadPriority(uint32_t & Priority, uint32_t handle)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (Handle: 0x%X)", handle);
    KernelObjectMap::const_iterator itr = m_KernelObjects.find(handle);
    if (itr != m_KernelObjects.end() && itr->second->GetHandleType() == CKernelObject::HandleType::Thread)
    {
        Priority = itr->second->GetSystemThreadPtr()->GetPriority();
        WriteTrace(TraceHleKernel, TraceInfo, "Done (Priority: 0x%X)", Priority);
        return RESULT_SUCCESS;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::ProcessSyncRequest(CService * Service, CIPCRequest & Request)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start");
    ResultCode call_result = RESULT_SUCCESS;

    if (Request.IsDomainRequest())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    WriteTrace(TraceHleKernel, TraceVerbose, "calling %s command: 0x%I64X", Service->Name(), Request.RequestHeader().Command);
    call_result = Service->CallMethod(Request);
    WriteTrace(TraceHleKernel, TraceInfo, "Done");
    return call_result;
}

ResultCode CHleKernel::ProcessSyncControl(CService * Service, CIPCRequest & Request)
{
    CIPCRequest::REQUEST_DATA & response_data = Request.ResponseData();

    switch (Request.RequestHeader().Command)
    {
    case ConvertCurrentObjectToDomain:
        if (!Service->IsDomain())
        {
            int32_t ObjectId = Service->ConvertToDomain();
            response_data.resize(sizeof(ObjectId));
            memcpy(response_data.data(), &ObjectId, sizeof(ObjectId));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        break;
    case QueryPointerBufferSize:
        response_data.insert(response_data.end(), { 0x00, 0x05, 0x00, 0x00 });
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::SendSyncRequest(uint32_t Handle)
{
    WriteTrace(TraceHleKernel, TraceDebug, "Start (Handle: 0x%X)", Handle);
    KernelObjectMap::iterator itr = m_KernelObjects.find(Handle);
    if (itr == m_KernelObjects.end() || itr->second->GetHandleType() != CKernelObject::HandleType::Service)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        WriteTrace(TraceHleKernel, TraceNotice, "Failed to find serivce for handle (Handle: 0x%X)", Handle);
        return RESULT_SUCCESS;
    }
    CService * Service = itr->second->GetServicePtr();
    CSystemThread * CurrentThread = m_System.SystemThread()->GetSystemThreadPtr();
    uint64_t tlsAddr = CurrentThread->TlsAddress();
    CIPCRequest Request(m_System, tlsAddr, Service);
    ResultCode call_result = RESULT_SUCCESS;

    WriteTrace(TraceHleKernel, TraceInfo, "Service: %s Command: %s RequestData Size: 0x%X", Service->Name(), CIPCRequest::CommandTypeName(Request.CommandType()), (uint32_t)Request.RequestData().size());
    switch (Request.CommandType())
    {
    case CIPCRequest::Command_Close:
        return IPC_ERR_REMOTE_PROCESS_DEAD;
    case CIPCRequest::Command_Request:
        call_result = ProcessSyncRequest(Service, Request);
        break;
    case CIPCRequest::Command_Control:
        call_result = ProcessSyncControl(Service, Request);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    Request.WriteResponse(call_result);
    WriteTrace(TraceHleKernel, TraceDebug, "Done (call_result: 0x%I64X)", call_result);
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::SignalProcessWideKey(uint64_t ptr, uint32_t value)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (ptr: 0x%I64X value: 0x%X)", ptr, value);
    typedef std::vector<CSystemThread*> ThreadList;

    ThreadList WaitingThreads;
    for (size_t i = 0, n = m_ThreadQueue.size(); i < n; i++)
    {
        CSystemThread * thread = m_ThreadQueue[i]->GetSystemThreadPtr();
        if (thread->GetCondVarWaitAddress() == ptr)
        {
            WaitingThreads.push_back(thread);
        }
    }
    WriteTrace(TraceHleKernel, TraceDebug, "Found %d waiting on address (0x%I64X)", ptr, value);

    struct PrioritySort
    {
        inline bool operator() (CSystemThread * lhs, const CSystemThread* rhs)
        {
            return lhs->GetPriority() < rhs->GetPriority();
        }
    };
    std::sort(WaitingThreads.begin(), WaitingThreads.end(), PrioritySort());

    std::size_t last = value != -1 ? std::min(WaitingThreads.size(), (std::size_t)value) : WaitingThreads.size();
    if (last == 0)
    {
        return RESULT_SUCCESS;
    }

    enum
    {
        MutexHasWaitersFlag = 0x40000000
    };

    for (size_t i = 0; i < last; i++)
    {
        CSystemThread * Thread = WaitingThreads[i];
        Thread->SetCondVarWaitAddress(0);

        CSystemThreadMemory & ThreadMemory = Thread->ThreadMemory();
        uint32_t MutexVal;
        if (!ThreadMemory.Read32(Thread->GetMutexWaitAddress(), MutexVal))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return RESULT_SUCCESS;
        }

        if (MutexVal == 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return RESULT_SUCCESS;
        }

        if (!ThreadMemory.Write32(Thread->GetMutexWaitAddress(), MutexVal | MutexHasWaitersFlag))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return RESULT_SUCCESS;
        }

        uint32_t OwnerHandle = MutexVal & 0xBFFFFFFF;
        KernelObjectMap::iterator OwnerThread = m_KernelObjects.find(OwnerHandle);
        if (OwnerThread == m_KernelObjects.end() || OwnerThread->second->GetHandleType() != CKernelObject::HandleType::Thread)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return RESULT_SUCCESS;
        }
        CSystemThread* Owner = OwnerThread->second->GetSystemThreadPtr();

        if (Thread->GetState() != CSystemThread::WaitCondVar)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return RESULT_SUCCESS;
        }
        //thread->InvalidateWakeupCallback();
        Thread->SetState(CSystemThread::WaitMutex);
        Owner->AddMutexWaiter(Thread);
    }
    WriteTrace(TraceHleKernel, TraceInfo, "Done (RESULT_SUCCESS)");
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::QueryMemory(CSystemThreadMemory & ThreadMemory, uint64_t MemoryInfoAddr, uint64_t QueryAddr)
{
    WriteTrace(TraceServiceCall, TraceVerbose, "Start (MemoryInfoAddr: 0x%I64X QueryAddr: 0x%I64X)", MemoryInfoAddr, QueryAddr);

    QueryMemoryInfo Info = { 0 };
    if (!ThreadMemory.GetMemoryInfo(QueryAddr, Info))
    {
        uint64_t AddrSpaceStart = 0x0000008000000000;
        uint64_t AddrSpaceSize = 0xFFFFFF8000000000;

        if (QueryAddr < AddrSpaceStart)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        Info.BaseAddress = AddrSpaceStart;
        Info.Size = AddrSpaceSize;
        Info.Type = MemoryType_Unmapped;
        Info.Permission = MemoryPermission_None;
    }

    WriteTrace(TraceServiceCall, TraceVerbose, "QueryAddr: 0x%I64X base_address: 0x%I64X size: 0x%I64X permission: %d type: %d", QueryAddr, Info.BaseAddress, Info.Size, Info.Permission, Info.Type);

    if (!ThreadMemory.WriteBytes(MemoryInfoAddr, (uint8_t *)&Info, sizeof(Info)))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    WriteTrace(TraceVerbose, TraceDebug, "Done");
    return RESULT_SUCCESS;
}

bool CHleKernel::AddSystemThread(uint32_t & ThreadHandle, const char * name, uint64_t entry_point, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId)
{
    ThreadHandle = GetNewHandle();
    CKernelObjectPtr ThreadObject(new CSystemThread(this, m_ProcessMemory, name, entry_point, ThreadHandle, CreateNewThreadID(), ThreadContext, StackTop, StackSize, Priority, ProcessorId));
    if (ThreadObject.get() == NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    CSystemThread * Thread = ThreadObject->GetSystemThreadPtr();
    Thread->SetState(CSystemThread::Ready);
    Thread->Reg().Set64(Arm64Opcode::ARM64_REG_X1, ThreadHandle);
    m_KernelObjects.insert(KernelObjectMap::value_type(ThreadHandle, ThreadObject));
    m_ThreadQueue.push_back(Thread);
    return true;
}

uint32_t CHleKernel::GetNewHandle()
{
    uint32_t handle = m_NextHandle++;
    return handle;
}

uint32_t CHleKernel::CreateNewThreadID()
{
    uint32_t ThreadId = m_NextThreadId++;
    return ThreadId;
}

const char * CHleKernel::GetInfoTypeName(GetInfoType Id)
{
    switch (Id)
    {
    case AllowedCpuIdBitmask: return "AllowedCpuIdBitmask";
    case AllowedThreadPrioBitmask: return "AllowedThreadPrioBitmask";
    case MapRegionBaseAddr: return "MapRegionBaseAddr";
    case MapRegionBaseSize: return "MapRegionBaseSize";
    case HeapRegionBaseAddr: return "HeapRegionBaseAddr";
    case HeapRegionSize: return "HeapRegionSize";
    case TotalMemoryAvailable: return "TotalMemoryAvailable";
    case TotalHeapUsage: return "TotalHeapUsage";
    case IsCurrentProcessBeingDebugged: return "IsCurrentProcessBeingDebugged";
    case RandomEntropy: return "RandomEntropy";
    case AddressSpaceBaseAddr: return "AddressSpaceBaseAddr";
    case AddressSpaceSize: return "AddressSpaceSize";
    case MapRegionAddress: return "MapRegionAddress";
    case MapRegionSize: return "MapRegionSize";
    }

    static stdstr unknown;
    unknown.Format("Unknown (0x%X)", Id);
    return unknown.c_str();
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
