#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\hle\Kernel\ResultCode.h>
#include <nxemu-core\hle\HleKernel.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>

CSystemThread::CSystemThread(CHleKernel * m_Kernel, CProcessMemory &ProcessMemory, const char * name, uint64_t entry_point, uint32_t /*ThreadHandle*/, uint32_t thread_id, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t /*ProcessorId*/) :
    CPUExecutor(m_ThreadMemory),
    m_Kernel(m_Kernel),
    m_ThreadMemory(ProcessMemory, this),
    m_ThreadId(thread_id),
	m_Priority(Priority),
    m_TlsAddress(ProcessMemory.GetTlsIoRegionBase()),
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
    case CHleKernel::svcQueryMemory:
        Result = m_Kernel->QueryMemory(m_ThreadMemory, m_Reg.Get64(Arm64Opcode::ARM64_REG_X0), m_Reg.Get64(Arm64Opcode::ARM64_REG_X2));
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

