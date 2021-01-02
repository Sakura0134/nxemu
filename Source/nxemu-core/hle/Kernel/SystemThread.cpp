#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\hle\Kernel\ResultCode.h>
#include <nxemu-core\hle\Kernel\HleKernel.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\Machine\Interpreter\InterpreterCPU.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\StdString.h>
#include <algorithm>

CSystemThread::CSystemThread(CSwitchSystem & System, CProcessMemory &ProcessMemory, const char * Name, uint64_t EntryPoint, uint32_t /*ThreadHandle*/, uint32_t ThreadId, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t /*ProcessorId*/) :
    CThread(stEmulationThread),
    m_System(System),
    m_ThreadMemory(ProcessMemory),
    m_State(ThreadState_Created),
    m_ThreadId(ThreadId),
    m_Priority(Priority),
    m_CondVarWaitAddress(0),
    m_MutexWaitAddress(0),
    m_WaitHandle(0),
    m_TlsAddress(ProcessMemory.GetTlsIoRegionBase()),
    m_CpuTicks(0),
    m_LockOwner(nullptr),
    m_Name(Name != nullptr ? Name : "")
{
    if (!m_ThreadMemory.Initialize(StackTop, StackSize, m_TlsAddress, 0x1000))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_CPU.reset(new CInterpreterCPU(System, *this));

    IRegisters & Reg = m_CPU->Reg();
    Reg.Set64(Arm64Opcode::ARM64_REG_SP, StackTop);
    Reg.Set64(Arm64Opcode::ARM64_REG_PC, EntryPoint);

    for (size_t i = 0; i < 31; i++)
    {
        Reg.Set64((Arm64Opcode::arm64_reg)(Arm64Opcode::ARM64_REG_X0 + i), 0);
    }
    Reg.Set64(Arm64Opcode::ARM64_REG_X0, ThreadContext);
    Reg.SetConditionFlags(false, false, false, false);
    Reg.Set64(Arm64Opcode::ARM64_REG_TPIDRRO_EL0, m_TlsAddress);
}

CSystemThread::~CSystemThread()
{
}

void CSystemThread::EmulationThread(void)
{
    if (m_State != ThreadState_Ready)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_System.m_SystemThread.Set(this);
    m_State = ThreadState_Running;
    m_CPU->Execute();
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CSystemThread::ServiceCall(uint32_t index)
{
    SvcCall Call = (SvcCall)index;
    WriteTrace(TraceServiceCall, TraceDebug, "Start (SvcCall: %s)", CHleKernel::GetSvcCallStr(Call));
    IRegisters & Reg = m_CPU->Reg();
    CHleKernel & HleKernel = m_System.HleKernel();

    ResultCode Result = RESULT_SUCCESS;
    bool VoidRes = false;

    switch (Call)
    {
    case SvcCall_SetHeapSize:
        {
            uint64_t HeapAddress = 0;
            Result = HleKernel.SetHeapSize(HeapAddress, Reg.Get64(Arm64Opcode::ARM64_REG_X1));
            Reg.Set64(Arm64Opcode::ARM64_REG_X1, HeapAddress);
        }
        break;
    case SvcCall_MapMemory:
        Result = HleKernel.MapMemory(Reg.Get64(Arm64Opcode::ARM64_REG_X0), Reg.Get64(Arm64Opcode::ARM64_REG_X1), Reg.Get64(Arm64Opcode::ARM64_REG_X2));
        break;
    case SvcCall_QueryMemory:
        Result = HleKernel.QueryMemory(m_ThreadMemory, Reg.Get64(Arm64Opcode::ARM64_REG_X0), Reg.Get64(Arm64Opcode::ARM64_REG_X2));
        break;
    case SvcCall_CreateThread:
        {
            uint32_t ThreadHandle = Reg.Get32(Arm64Opcode::ARM64_REG_W1);
            Result = HleKernel.CreateThread(ThreadHandle, Reg.Get64(Arm64Opcode::ARM64_REG_X1), Reg.Get64(Arm64Opcode::ARM64_REG_X2), Reg.Get64(Arm64Opcode::ARM64_REG_X3), 0, Reg.Get32(Arm64Opcode::ARM64_REG_W4), Reg.Get32(Arm64Opcode::ARM64_REG_W5));
            Reg.Set32(Arm64Opcode::ARM64_REG_W1, ThreadHandle);
        }
        break;
    case SvcCall_StartThread:
        Result = HleKernel.StartThread(Reg.Get32(Arm64Opcode::ARM64_REG_W0));
        break;
    case SvcCall_GetThreadPriority:
        {
            uint32_t Priority = Reg.Get32(Arm64Opcode::ARM64_REG_W1);
            Result = HleKernel.GetThreadPriority(Priority, Priority);
            Reg.Set32(Arm64Opcode::ARM64_REG_W1, Priority);
        }
        break;
    case SvcCall_CloseHandle:
        Result = HleKernel.CloseHandle(Reg.Get32(Arm64Opcode::ARM64_REG_W0));
        break;
    case SvcCall_WaitSynchronization:
        {
            uint32_t HandleIndex = Reg.Get32(Arm64Opcode::ARM64_REG_W1);
            Result = HleKernel.WaitSynchronization(m_ThreadMemory, HandleIndex, Reg.Get64(Arm64Opcode::ARM64_REG_X1), Reg.Get32(Arm64Opcode::ARM64_REG_W2), Reg.Get64(Arm64Opcode::ARM64_REG_X3));
            Reg.Set32(Arm64Opcode::ARM64_REG_W1, HandleIndex);
        }
        break;
    case SvcCall_ArbitrateUnlock:
        Result = HleKernel.ArbitrateUnlock(Reg.Get64(Arm64Opcode::ARM64_REG_X0));
        break;
    case SvcCall_WaitProcessWideKeyAtomic:
        Result = HleKernel.WaitProcessWideKeyAtomic(Reg.Get64(Arm64Opcode::ARM64_REG_X0), Reg.Get64(Arm64Opcode::ARM64_REG_X1), Reg.Get32(Arm64Opcode::ARM64_REG_W2), Reg.Get64(Arm64Opcode::ARM64_REG_X3));
        break;
    case SvcCall_SignalProcessWideKey:
        Result = HleKernel.SignalProcessWideKey(Reg.Get64(Arm64Opcode::ARM64_REG_X0), Reg.Get32(Arm64Opcode::ARM64_REG_W1));
        break;
    case SvcCall_ConnectToNamedPort:
        {
            uint32_t SessionHandle = Reg.Get32(Arm64Opcode::ARM64_REG_W1);
            Result = HleKernel.ConnectToNamedPort(m_ThreadMemory, SessionHandle, Reg.Get64(Arm64Opcode::ARM64_REG_X1));
            Reg.Set32(Arm64Opcode::ARM64_REG_W1, SessionHandle);
        }
        break;
    case SvcCall_SendSyncRequest:
        Result = HleKernel.SendSyncRequest(Reg.Get32(Arm64Opcode::ARM64_REG_W0));
        break;
    case SvcCall_GetInfo:
        {
            uint64_t Info = Reg.Get64(Arm64Opcode::ARM64_REG_X1);
            Result = HleKernel.GetInfo(Info, (CHleKernel::GetInfoType)Reg.Get32(Arm64Opcode::ARM64_REG_W1), Reg.Get32(Arm64Opcode::ARM64_REG_W2), Reg.Get64(Arm64Opcode::ARM64_REG_X3));
            Reg.Set64(Arm64Opcode::ARM64_REG_X1, Info);
        }
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    if (!VoidRes)
    {
        Reg.Set64(Arm64Opcode::ARM64_REG_X0, Result.Raw);
    }
    WriteTrace(TraceServiceCall, TraceDebug, VoidRes ? "Done" : "Done (Result: %s)", ResultCodeStr(Result));
}

void CSystemThread::ResetSyncEvent(void)
{
    m_SyncEvent.Reset();
}

void CSystemThread::TriggerSyncEvent(void)
{
    m_SyncEvent.Trigger();
}

void CSystemThread::WaitSyncEvent(int32_t iWaitTime)
{
    m_SyncEvent.IsTriggered(iWaitTime);
}

const char * CSystemThread::GetStateName() const
{
    switch (m_State)
    {
    case ThreadState_Unknown: return "Unknown";
    case ThreadState_Created: return "Created";
    case ThreadState_Ready: return "Ready";
    case ThreadState_Running: return "Running";
    case ThreadState_WaitCondVar: return "Wait Cond Var";
    case ThreadState_WaitMutex: return "Wait Mutex";
    }
    static stdstr UnknownSate;
    UnknownSate.Format("Unknown %d", m_State);
    return UnknownSate.c_str();
}

IRegisters& CSystemThread::Reg(void)
{
    return m_CPU->Reg();
}

void CSystemThread::AddMutexWaiter(CSystemThread * thread)
{
    if (thread->m_LockOwner == this)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    if (thread->m_LockOwner != nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    MutexWaitingThreads::const_iterator itr = std::find(m_WaitMutexThreads.begin(), m_WaitMutexThreads.end(), thread);
    if (itr != m_WaitMutexThreads.end())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    struct priority_compare
    {
        explicit priority_compare(const CSystemThread * _a) : a(_a) { }
        bool operator()(const CSystemThread * b) const { return a->GetPriority() > b->GetPriority(); }

    private:
        const CSystemThread * a;
    };

    MutexWaitingThreads::const_iterator InsertionPoint = std::find_if(m_WaitMutexThreads.begin(), m_WaitMutexThreads.end(), priority_compare(thread));
    m_WaitMutexThreads.insert(InsertionPoint, thread);
    thread->m_LockOwner = this;
}

void CSystemThread::RemoveMutexWaiter(CSystemThread * thread)
{
    if (thread->m_LockOwner != this)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    MutexWaitingThreads::const_iterator itr = std::find(m_WaitMutexThreads.begin(), m_WaitMutexThreads.end(), thread);
    if (itr == m_WaitMutexThreads.end())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_WaitMutexThreads.erase(itr);
    thread->m_LockOwner = nullptr;
}

void CSystemThread::SetCondVarWaitAddress(uint64_t CondVarWaitAddress)
{
    m_CondVarWaitAddress = CondVarWaitAddress;
}

void CSystemThread::SetMutexWaitAddress(uint64_t MutexWaitAddress)
{
    m_MutexWaitAddress = MutexWaitAddress;
}

void CSystemThread::SetWaitHandle(uint32_t WaitHandle)
{
    m_WaitHandle = WaitHandle;
}

void CSystemThread::SetState(ThreadState state)
{
    m_State = state;
}

void CSystemThread::SetLockOwner(CSystemThread * LockThread)
{
    m_LockOwner = LockThread;
}

void CSystemThread::Start(void)
{
    CThread::Start(this);
}
