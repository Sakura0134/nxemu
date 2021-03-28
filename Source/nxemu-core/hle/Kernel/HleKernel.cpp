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
    m_Display(System),
    m_System(System),
    m_ProcessMemory(ProcessMemory),
    m_SM(System),
    m_NextHandle(0x8002),
    m_NextThreadId(1)
{
    CRandom EntropyRandomize(g_Settings->LoadBool(HleKernel_RandomizeEntropy) ? (uint32_t)time(nullptr) : g_Settings->LoadDword(HleKernel_RandomizeSeed));
    for (uint32_t i = 0, n = sizeof(m_RandomEntropy) / sizeof(m_RandomEntropy[0]); i < n; i++)
    {
        m_RandomEntropy[i] = ((uint64_t)EntropyRandomize.next() << 32) | EntropyRandomize.next();
    }

    m_NamedPorts.insert(NamedPortList::value_type("sm:", &m_SM));
}

CHleKernel::~CHleKernel()
{
}

ResultCode CHleKernel::CreateThread(uint32_t & ThreadHandle, uint64_t EntryPoint, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (EntryPoint: 0x%I64X ThreadContext: 0x%I64X StackTop: 0x%I64X StackSize: 0x%X Priority: 0x%X ProcessorId: 0x%X)", EntryPoint, ThreadContext, StackTop, StackSize, Priority, ProcessorId);
    CGuard Guard(m_CS);

    if (Priority > 0x3F)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }

    uint32_t Handle = GetNewHandle();
    std::string name = stdstr_f("Thread[%X-%X]", EntryPoint, Handle);
    CSystemThread * thread = new CSystemThread(m_System, m_ProcessMemory, name.c_str(), EntryPoint, Handle, CreateNewThreadID(), ThreadContext, StackTop, StackSize, Priority, ProcessorId);
    if (thread == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }

    m_KernelObjects.insert(KernelObjectMap::value_type(Handle, thread));
    ThreadHandle = Handle;
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::StartThread(uint32_t ThreadHandle)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (ThreadHandle: 0x%X)", ThreadHandle);
    CGuard Guard(m_CS);

    KernelObjectMap::const_iterator itr = m_KernelObjects.find(ThreadHandle);
    if (itr == m_KernelObjects.end() || itr->second->GetHandleType() != KernelObjectHandleType_Thread)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return ERR_INVALID_HANDLE;
    }
    CSystemThread * thread = itr->second->GetSystemThreadPtr();
    if (thread->GetState() != CSystemThread::ThreadState_Created)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return ERR_INVALID_HANDLE;
    }
    thread->SetState(CSystemThread::ThreadState_Ready);
    thread->Start();
    return RESULT_SUCCESS;
}

uint32_t CHleKernel::AddKernelObject(CKernelObject * Object)
{
    if (Object == nullptr)
    {
        return (uint32_t)-1;
    }
    CGuard Guard(m_CS);
    uint32_t Handle = GetNewHandle();
    m_KernelObjects.insert(KernelObjectMap::value_type(Handle, Object));
    return Handle;
}

KernelObjectMap CHleKernel::KernelObjects(void)
{
    CGuard Guard(m_CS);
    KernelObjectMap Objects;
    Objects = m_KernelObjects;
    return Objects;
}

ResultCode CHleKernel::ArbitrateLock(uint32_t Handle, uint64_t MutexAddress, uint32_t Tag)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (handle: 0x%X, MutexAddress: 0x%I64X, handle: 0x%X)", Handle, MutexAddress, Tag);

    CSystemThread * CurrentThread = m_System.SystemThread()->GetSystemThreadPtr();
    if (CurrentThread == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }

    CKernelObjectPtr ThreadObject, RequestingThreadObject;
    {
        CGuard Guard(m_CS);
        KernelObjectMap::const_iterator itr = m_KernelObjects.find(Handle);
        if (itr == m_KernelObjects.end() || itr->second->GetHandleType() != KernelObjectHandleType_Thread)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return ERR_INVALID_HANDLE;
        }
        ThreadObject.reset(itr->second.get());
        itr = m_KernelObjects.find(Tag);
        if (itr == m_KernelObjects.end() || itr->second->GetHandleType() != KernelObjectHandleType_Thread)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return ERR_INVALID_HANDLE;
        }
        RequestingThreadObject.reset(itr->second.get());
    }
    CSystemThread * HoldingThread = ThreadObject->GetSystemThreadPtr();
    CSystemThread * RequestingThread = RequestingThreadObject->GetSystemThreadPtr();
    if (RequestingThread != CurrentThread || CurrentThread == HoldingThread)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }

    return m_Mutex.Acquire(MutexAddress, Handle, HoldingThread, Tag, RequestingThread);
}

ResultCode CHleKernel::ArbitrateUnlock(uint64_t MutexAddress)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (MutexAddress: 0x%I64X)", MutexAddress);
    CGuard Guard(m_CS);

    if ((MutexAddress & 0x3) != 0)
    {
        return ERR_INVALID_ADDRESS;
    }

    CSystemThread * CurrentThread = m_System.SystemThread()->GetSystemThreadPtr();
    if (CurrentThread == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }

    return m_Mutex.Release(MutexAddress, CurrentThread);
}

ResultCode CHleKernel::CloseHandle(uint32_t Handle)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (Handle: 0x%X)", Handle);
    CGuard Guard(m_CS);

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
    CGuard Guard(m_CS);

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

ResultCode CHleKernel::CreateTransferMemory(uint32_t & TransferMemoryHandle, uint64_t Addr, uint64_t Size, MemoryPermission Permission)
{
    WriteTrace(TraceHleKernel, TraceNotice, "Start (Addr: 0x%I64X Size: 0x%I64X Permissions: 0x%X)", Addr, Size, Permission);
    CGuard Guard(m_CS);

    TransferMemoryHandle = 0;

    if ((Addr & 0xFFF) != 0)
    {
        WriteTrace(TraceHleKernel, TraceWarning, "Addr (0x%I64X) is not page aligned", Addr);
        WriteTrace(TraceHleKernel, TraceInfo, "Done (TransferMemoryHandle: 0x%I64X Return: ERR_INVALID_ADDRESS)", TransferMemoryHandle);
        return ERR_INVALID_ADDRESS;
    }

    if ((Size & 0xFFF) != 0 || Size == 0)
    {
        WriteTrace(TraceHleKernel, TraceWarning, "Size (0x%I64X) is not page aligned or equal to zero!", Size);
        WriteTrace(TraceHleKernel, TraceInfo, "Done (TransferMemoryHandle: 0x%I64X Return: ERR_INVALID_ADDRESS)", TransferMemoryHandle);
        return ERR_INVALID_ADDRESS;
    }

    if (Addr + Size <= Addr)
    {
        WriteTrace(TraceHleKernel, TraceWarning, "Address and size cause overflow! (Addr=0x%I64X, Size=0x%I64X)", Addr, Size);
        WriteTrace(TraceHleKernel, TraceInfo, "Done (TransferMemoryHandle: 0x%I64X Return: ERR_INVALID_ADDRESS_STATE)", TransferMemoryHandle);
        return ERR_INVALID_ADDRESS_STATE;
    }

    if (Permission != MemoryPermission_None && Permission != MemoryPermission_Read && Permission != MemoryPermission_ReadWrite)
    {
        WriteTrace(TraceHleKernel, TraceWarning, "Invalid memory permissions (Permission: 0x%X)", Permission);
        WriteTrace(TraceHleKernel, TraceInfo, "Done (TransferMemoryHandle: 0x%I64X Return: ERR_INVALID_MEMORY_PERMISSIONS)", TransferMemoryHandle);
        return ERR_INVALID_MEMORY_PERMISSIONS;
    }

    CTransferMemory * TransferMemory = new CTransferMemory(Addr, Size, Permission);
    if (TransferMemory == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    TransferMemoryHandle = AddKernelObject(TransferMemory);
    WriteTrace(TraceHleKernel, TraceInfo, "Done (TransferMemoryHandle: 0x%I64X Return: RESULT_SUCCESS)", TransferMemoryHandle);
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::GetInfo(uint64_t & Info, GetInfoType InfoType, uint32_t handle, uint64_t SubId)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (InfoType: %s handle: 0x%X SubId: 0x%I64X)", GetInfoTypeName(InfoType), handle, SubId);
    CGuard Guard(m_CS);
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
    CGuard Guard(m_CS);

    KernelObjectMap::const_iterator itr = m_KernelObjects.find(handle);
    if (itr != m_KernelObjects.end() && itr->second->GetHandleType() == KernelObjectHandleType_Thread)
    {
        Priority = itr->second->GetSystemThreadPtr()->GetPriority();
        WriteTrace(TraceHleKernel, TraceInfo, "Done (Priority: 0x%X)", Priority);
        return RESULT_SUCCESS;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::MapMemory(uint64_t DstAddress, uint64_t SrcAddress, uint64_t Size)
{
    WriteTrace(TraceHleKernel, TraceDebug, "Start (DstAddress: 0x%I64X SrcAddress: 0x%I64X Size: 0x%I64X)", DstAddress, SrcAddress, Size);
    CGuard Guard(m_CS);

    if (!m_ProcessMemory.MirrorMemory(DstAddress, SrcAddress, Size))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::ProcessSyncRequest(CService * Service, CIPCRequest & Request)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start");
    ResultCode call_result = RESULT_SUCCESS;

    if (Request.IsDomainRequest())
    {
        const CIPCRequest::IpcDomainMessage & DomainMessage = Request.DomainMessage();

        switch (DomainMessage.Command)
        {
        case 1:
            if (!Service->FindDomainService(DomainMessage.ObjectID, Service))
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            break;
        case 2:
            //CloseHandle(DomainMessage.ObjectID);
            return RESULT_SUCCESS;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
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
    case CloneCurrentObject:
        Request.AddResponseHandlesToMove(AddKernelObject(Service));
        Request.SetDomainRequest(false);
        response_data.insert(response_data.end(), { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
        break;
    case CloneCurrentObjectEx:
        Request.AddResponseHandlesToMove(AddKernelObject(Service));
        Request.SetDomainRequest(false);
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
    CService * Service = nullptr;
    
    {
        CGuard Guard(m_CS);

        KernelObjectMap::iterator itr = m_KernelObjects.find(Handle);
        if (itr == m_KernelObjects.end() || itr->second->GetHandleType() != KernelObjectHandleType_Service)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            WriteTrace(TraceHleKernel, TraceNotice, "Failed to find serivce for handle (Handle: 0x%X)", Handle);
            return RESULT_SUCCESS;
        }
        Service = itr->second->GetServicePtr();
    }
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

ResultCode CHleKernel::SetHeapSize(uint64_t & HeapAddress, uint64_t size)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (size: 0x%I64X)", size);
    CGuard Guard(m_CS);

    if ((size % 0x200000) != 0 || size >= 0x200000000)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }

    if (!m_ProcessMemory.SetHeapSize(size))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    HeapAddress = m_ProcessMemory.GetHeapRegionBaseAddr();
    WriteTrace(TraceHleKernel, TraceInfo, "Done (HeapAddress: 0x%I64X)", HeapAddress);
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::SignalProcessWideKey(uint64_t ptr, uint32_t value)
{
    WriteTrace(TraceHleKernel, TraceInfo, "Start (ptr: 0x%I64X value: 0x%X)", ptr, value);
    CGuard Guard(m_CS);

    typedef std::vector<CSystemThread*> ThreadList;

    ThreadList WaitingThreads;
    for (KernelObjectMap::const_iterator itr = m_KernelObjects.begin(); itr != m_KernelObjects.end(); itr++)
    {
        if (itr->second->GetHandleType() != KernelObjectHandleType_Thread)
        {
            continue;
        }
        CSystemThread * Thread = itr->second->GetSystemThreadPtr();
        if (Thread->GetCondVarWaitAddress() == ptr)
        {
            WaitingThreads.push_back(Thread);
        }
    }

    if (WaitingThreads.size() == 0)
    {
        return RESULT_SUCCESS;
    }

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
        if (OwnerThread == m_KernelObjects.end() || OwnerThread->second->GetHandleType() != KernelObjectHandleType_Thread)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return RESULT_SUCCESS;
        }
        CSystemThread* Owner = OwnerThread->second->GetSystemThreadPtr();

        if (Thread->GetState() != CSystemThread::ThreadState_WaitCondVar)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return RESULT_SUCCESS;
        }
        Thread->SetState(CSystemThread::ThreadState_WaitMutex);
        Owner->AddMutexWaiter(Thread);
    }
    WriteTrace(TraceHleKernel, TraceInfo, "Done (RESULT_SUCCESS)");
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::QueryMemory(CSystemThreadMemory & ThreadMemory, uint64_t MemoryInfoAddr, uint64_t QueryAddr)
{
    WriteTrace(TraceServiceCall, TraceVerbose, "Start (MemoryInfoAddr: 0x%I64X QueryAddr: 0x%I64X)", MemoryInfoAddr, QueryAddr);
    CGuard Guard(m_CS);

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
    CGuard Guard(m_CS);

    ThreadHandle = GetNewHandle();
    CKernelObjectPtr ThreadObject(new CSystemThread(m_System, m_ProcessMemory, name, entry_point, ThreadHandle, CreateNewThreadID(), ThreadContext, StackTop, StackSize, Priority, ProcessorId));
    if (ThreadObject.get() == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    CSystemThread * Thread = ThreadObject->GetSystemThreadPtr();
    Thread->SetState(CSystemThread::ThreadState_Ready);
    Thread->Reg().Set64(Arm64Opcode::ARM64_REG_X1, ThreadHandle);
    m_KernelObjects.insert(KernelObjectMap::value_type(ThreadHandle, ThreadObject));
    return true;
}

ResultCode CHleKernel::WaitProcessWideKeyAtomic(uint64_t MutexAddress, uint64_t ConditionAddr, uint32_t ThreadHandle, uint64_t timeout)
{
    if ((MutexAddress & 0x3) != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return ERR_INVALID_ADDRESS;
    }

    CKernelObjectPtr CurrentThread(nullptr);
    CSystemThread * Thread = nullptr;
    {
        CGuard Guard(m_CS);

        KernelObjectMap::const_iterator itr = m_KernelObjects.find(ThreadHandle);
        if (itr == m_KernelObjects.end() || itr->second->GetHandleType() != KernelObjectHandleType_Thread)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return RESULT_SUCCESS;
        }
        CurrentThread.reset(m_System.SystemThread());
        if (CurrentThread.get() == nullptr)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return RESULT_SUCCESS;
        }
        Thread = CurrentThread->GetSystemThreadPtr();
        ResultCode ReleaseResult = m_Mutex.Release(MutexAddress, Thread);
        if (ReleaseResult.IsError())
        {
            return ReleaseResult.Raw;
        }
        Thread->SetCondVarWaitAddress(ConditionAddr);
        Thread->SetMutexWaitAddress(MutexAddress);
        Thread->SetWaitHandle(ThreadHandle);
        Thread->SetState(CSystemThread::ThreadState_WaitCondVar);
        Thread->ResetSyncEvent();
    }

    Thread->WaitSyncEvent((int32_t)timeout);
    Thread->SetState(CSystemThread::ThreadState_Running);
    return RESULT_SUCCESS;
}

ResultCode CHleKernel::WaitSynchronization(CSystemThreadMemory & ThreadMemory, uint32_t & HandleIndex, uint64_t HandlesPtr, uint32_t HandlesNum, uint64_t Timeout)
{
    CGuard Guard(m_CS);

    if (!ThreadMemory.IsValidAddress(HandlesPtr))
    {
        return ERR_INVALID_POINTER;
    }

    enum { MaxHandles = 0x40 };
    if (HandlesNum > MaxHandles)
    {
        return ERR_OUT_OF_RANGE;
    }

    std::vector<CKernelObjectPtr> Events;
    for (uint32_t i = 0; i < HandlesNum; ++i)
    {
        uint32_t Handle;
        if (!ThreadMemory.Read32(HandlesPtr + (i * sizeof(uint32_t)), Handle))
        {
            return ERR_INVALID_HANDLE;
        }
        KernelObjectMap::const_iterator itr = m_KernelObjects.find(Handle);
        if (itr == m_KernelObjects.end() || itr->second->GetHandleType() != KernelObjectHandleType_Event)
        {
            return ERR_INVALID_HANDLE;
        }
        Events.push_back(itr->second);
    }

    for (size_t i = 0, n = Events.size(); i < n; ++i)
    {
        if (Events[i]->GetKEventPtr()->ShouldWait())
        {
            continue;
        }
        HandleIndex = (uint32_t)i;
        return RESULT_SUCCESS;
    }
    if (Timeout == 0)
    {
        return RESULT_TIMEOUT;
    }
    CSystemThread * CurrentThread = m_System.SystemThread()->GetSystemThreadPtr();
    if (CurrentThread == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    int32_t EventTriggered = CurrentThread->WaitEvent(Events, Timeout);
    if (EventTriggered >= 0)
    {
        HandleIndex = (uint32_t)EventTriggered;
        return RESULT_SUCCESS;
    }
    return RESULT_TIMEOUT;
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

const char * CHleKernel::GetSvcCallStr(SvcCall Call)
{
    switch (Call)
    {
    case SvcCall_SetHeapSize: return "SetHeapSize";
    case SvcCall_SetMemoryPermission: return "SetMemoryPermission";
    case SvcCall_SetMemoryAttribute: return "SetMemoryAttribute";
    case SvcCall_MapMemory: return "MapMemory";
    case SvcCall_UnmapMemory: return "UnmapMemory";
    case SvcCall_QueryMemory: return "QueryMemory";
    case SvcCall_ExitProcess: return "ExitProcess";
    case SvcCall_CreateThread: return "CreateThread";
    case SvcCall_StartThread: return "StartThread";
    case SvcCall_ExitThread: return "ExitThread";
    case SvcCall_SleepThread: return "SleepThread";
    case SvcCall_GetThreadPriority: return "GetThreadPriority";
    case SvcCall_SetThreadPriority: return "SetThreadPriority";
    case SvcCall_GetThreadCoreMask: return "GetThreadCoreMask";
    case SvcCall_SetThreadCoreMask: return "SetThreadCoreMask";
    case SvcCall_GetCurrentProcessorNumber: return "GetCurrentProcessorNumber";
    case SvcCall_SignalEvent: return "SignalEvent";
    case SvcCall_ClearEvent: return "ClearEvent";
    case SvcCall_MapSharedMemory: return "MapSharedMemory";
    case SvcCall_UnmapSharedMemory: return "UnmapSharedMemory";
    case SvcCall_CreateTransferMemory: return "CreateTransferMemory";
    case SvcCall_CloseHandle: return "CloseHandle";
    case SvcCall_ResetSignal: return "ResetSignal";
    case SvcCall_WaitSynchronization: return "WaitSynchronization";
    case SvcCall_CancelSynchronization: return "CancelSynchronization";
    case SvcCall_ArbitrateLock: return "ArbitrateLock";
    case SvcCall_ArbitrateUnlock: return "ArbitrateUnlock";
    case SvcCall_WaitProcessWideKeyAtomic: return "WaitProcessWideKeyAtomic";
    case SvcCall_SignalProcessWideKey: return "SignalProcessWideKey";
    case SvcCall_GetSystemTick: return "GetSystemTick";
    case SvcCall_ConnectToNamedPort: return "ConnectToNamedPort";
    case SvcCall_SendSyncRequestLight: return "SendSyncRequestLight";
    case SvcCall_SendSyncRequest: return "SendSyncRequest";
    case SvcCall_SendSyncRequestWithUserBuffer: return "SendSyncRequestWithUserBuffer";
    case SvcCall_SendAsyncRequestWithUserBuffer: return "SendAsyncRequestWithUserBuffer";
    case SvcCall_GetProcessId: return "GetProcessId";
    case SvcCall_GetThreadId: return "GetThreadId";
    case SvcCall_Break: return "Break";
    case SvcCall_OutputDebugString: return "OutputDebugString";
    case SvcCall_ReturnFromException: return "ReturnFromException";
    case SvcCall_GetInfo: return "GetInfo";
    case SvcCall_FlushEntireDataCache: return "FlushEntireDataCache";
    case SvcCall_FlushDataCache: return "FlushDataCache";
    case SvcCall_MapPhysicalMemory: return "MapPhysicalMemory";
    case SvcCall_UnmapPhysicalMemory: return "UnmapPhysicalMemory";
    case SvcCall_GetDebugFutureThreadInfo: return "GetDebugFutureThreadInfo";
    case SvcCall_GetLastThreadInfo: return "GetLastThreadInfo";
    }
    static stdstr unknown;
    unknown.Format("Unknown (0x%X)", Call);
    return unknown.c_str();
}
