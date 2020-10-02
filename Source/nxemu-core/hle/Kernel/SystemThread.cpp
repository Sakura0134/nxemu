#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\hle\Kernel\ResultCode.h>
#include <nxemu-core\hle\Kernel\HleKernel.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <algorithm>

CSystemThread::CSystemThread(CHleKernel * m_Kernel, CProcessMemory &ProcessMemory, const char * name, uint64_t entry_point, uint32_t /*ThreadHandle*/, uint32_t thread_id, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t /*ProcessorId*/) :
    CInterpreterCPU(m_ThreadMemory),
    m_Kernel(m_Kernel),
    m_ThreadMemory(ProcessMemory, this),
    m_State(Created),
    m_ThreadId(thread_id),
	m_Priority(Priority),
    m_CondVarWaitAddress(0),
    m_MutexWaitAddress(0),
    m_TlsAddress(ProcessMemory.GetTlsIoRegionBase()),
    m_LockOwner(NULL),
	m_Name(name != NULL ? name : "")
{
    if (!m_ThreadMemory.Initialize(StackTop, StackSize, m_TlsAddress, 0x1000))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    m_Reg.Set64(Arm64Opcode::ARM64_REG_SP, StackTop);
    m_Reg.Set64(Arm64Opcode::ARM64_REG_PC, entry_point);

    for (size_t i = 0; i < 31; i++)
    {
        m_Reg.Set64((Arm64Opcode::arm64_reg)(Arm64Opcode::ARM64_REG_X0 + i), 0);
    }
    m_Reg.Set64(Arm64Opcode::ARM64_REG_X0, ThreadContext);
    m_Reg.SetConditionFlags(false, false, false, false);
    m_Reg.Set64(Arm64Opcode::ARM64_REG_TPIDRRO_EL0, m_TlsAddress);
}

CSystemThread::~CSystemThread()
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CSystemThread::ServiceCall(uint32_t index)
{
    CHleKernel::SvcGetCall svcCall = (CHleKernel::SvcGetCall)index;
    WriteTrace(TraceServiceCall, TraceDebug, "Start (SvcCall: %s)", CHleKernel::SvcGetCallStr(svcCall));

    ResultCode Result = RESULT_SUCCESS;
    bool VoidRes = false;

    switch (svcCall)
    {
    case CHleKernel::svcSetHeapSize:
        {
            uint64_t HeapAddress = 0;
            Result = m_Kernel->SetHeapSize(HeapAddress, m_Reg.Get64(Arm64Opcode::ARM64_REG_X1));
            m_Reg.Set64(Arm64Opcode::ARM64_REG_X1, HeapAddress);
        }
        break;
    case CHleKernel::svcQueryMemory:
        Result = m_Kernel->QueryMemory(m_ThreadMemory, m_Reg.Get64(Arm64Opcode::ARM64_REG_X0), m_Reg.Get64(Arm64Opcode::ARM64_REG_X2));
        break;
    case CHleKernel::svcGetThreadPriority:
        {
            uint32_t Priority = m_Reg.Get32(Arm64Opcode::ARM64_REG_W1);
            Result = m_Kernel->GetThreadPriority(Priority, Priority);
            m_Reg.Set32(Arm64Opcode::ARM64_REG_W1, Priority);
        }
        break;
    case CHleKernel::svcCloseHandle:
        Result = m_Kernel->CloseHandle(m_Reg.Get32(Arm64Opcode::ARM64_REG_W0));
        break;
    case CHleKernel::svcWaitSynchronization:
        {
            uint32_t HandleIndex = m_Reg.Get32(Arm64Opcode::ARM64_REG_W1);
            Result = m_Kernel->WaitSynchronization(m_ThreadMemory, HandleIndex, m_Reg.Get64(Arm64Opcode::ARM64_REG_X1), m_Reg.Get32(Arm64Opcode::ARM64_REG_W2), m_Reg.Get64(Arm64Opcode::ARM64_REG_X3));
            m_Reg.Set32(Arm64Opcode::ARM64_REG_W1, HandleIndex);
        }
        break;
    case CHleKernel::svcSignalProcessWideKey:
        Result = m_Kernel->SignalProcessWideKey(m_Reg.Get64(Arm64Opcode::ARM64_REG_X0), m_Reg.Get32(Arm64Opcode::ARM64_REG_W1));
        break;
    case CHleKernel::svcConnectToNamedPort:
        {
            uint32_t SessionHandle = m_Reg.Get32(Arm64Opcode::ARM64_REG_W1);
            Result = m_Kernel->ConnectToNamedPort(m_ThreadMemory, SessionHandle, m_Reg.Get64(Arm64Opcode::ARM64_REG_X1));
            m_Reg.Set32(Arm64Opcode::ARM64_REG_W1, SessionHandle);
        }
        break;
    case CHleKernel::svcSendSyncRequest:
        Result = m_Kernel->SendSyncRequest(m_Reg.Get32(Arm64Opcode::ARM64_REG_W0));
        break;
    case CHleKernel::svcGetInfo:
        {
            uint64_t Info = m_Reg.Get64(Arm64Opcode::ARM64_REG_X1);
            Result = m_Kernel->GetInfo(Info, (CHleKernel::GetInfoType)m_Reg.Get32(Arm64Opcode::ARM64_REG_W1), m_Reg.Get32(Arm64Opcode::ARM64_REG_W2), m_Reg.Get64(Arm64Opcode::ARM64_REG_X3));
            m_Reg.Set64(Arm64Opcode::ARM64_REG_X1, Info);
        }
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    if (!VoidRes)
    {
        m_Reg.Set64(Arm64Opcode::ARM64_REG_X0, Result.Raw);
    }
    WriteTrace(TraceServiceCall, TraceDebug, VoidRes ? "Done" : "Done (Result: %s)", ResultCodeStr(Result));
}

void CSystemThread::AddMutexWaiter(CSystemThread * thread)
{
    if (thread->m_LockOwner == this) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    if (thread->m_LockOwner != NULL)
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

void CSystemThread::SetCondVarWaitAddress(uint64_t CondVarWaitAddress)
{
    m_CondVarWaitAddress = CondVarWaitAddress;
}

void CSystemThread::SetMutexWaitAddress(uint64_t MutexWaitAddress)
{
    m_MutexWaitAddress = MutexWaitAddress;
}

void CSystemThread::SetState(ThreadState state)
{
    m_State = state;
}
