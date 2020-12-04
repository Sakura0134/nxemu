#pragma once
#include <nxemu-core\Machine\Interpreter\InterpreterCPU.h>
#include <nxemu-core\hle\Memory\SystemThreadMemory.h>
#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <Common\stdtypes.h>
#include <map>

class CHleKernel;

class CSystemThread :
    public CInterpreterCPU,
    public CKernelObject
{
public:
    typedef std::vector<CSystemThread *> MutexWaitingThreads;
    
    enum ThreadState
    {
        ThreadState_Unknown,
        ThreadState_Created,
        ThreadState_Ready,
        ThreadState_Running,
        ThreadState_WaitCondVar,
        ThreadState_WaitMutex,
    };

    CSystemThread(CHleKernel * m_Kernel, CProcessMemory &ProcessMemory, const char * name, uint64_t entry_point, uint32_t ThreadHandle, uint32_t thread_id, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId);
    ~CSystemThread();

    inline CSystemThreadMemory & ThreadMemory() { return m_ThreadMemory; }
    inline uint32_t GetPriority() const { return m_Priority; }
    inline ThreadState GetState() const { return m_State; }
    inline uint64_t GetCondVarWaitAddress() const { return m_CondVarWaitAddress; }
    inline uint64_t GetMutexWaitAddress() const { return m_MutexWaitAddress; }
    inline CSystemThread * GetLockOwner() const { return m_LockOwner; }
    inline uint64_t TlsAddress() const { return m_TlsAddress; }

    void AddMutexWaiter(CSystemThread * thread);
    void SetCondVarWaitAddress(uint64_t CondVarWaitAddress);
    void SetMutexWaitAddress(uint64_t MutexWaitAddress);
    void SetState(ThreadState state);

private:
    CSystemThread();
    CSystemThread(const CSystemThread&);
    CSystemThread& operator=(const CSystemThread&);

    HandleType GetHandleType() const { return Thread; }
    CSystemThread * GetSystemThreadPtr(void) { return this; }

    void ServiceCall(uint32_t index);
        
    CHleKernel * m_Kernel;
    CSystemThreadMemory m_ThreadMemory;
    MutexWaitingThreads m_WaitMutexThreads;
    ThreadState m_State;
    uint32_t m_ThreadId;
	uint32_t m_Priority;
    uint64_t m_CondVarWaitAddress;
    uint64_t m_MutexWaitAddress;
    uint64_t m_TlsAddress;
    CSystemThread * m_LockOwner;
	std::string m_Name;
};

typedef std::map<uint32_t, CSystemThread*> SystemThreadList;