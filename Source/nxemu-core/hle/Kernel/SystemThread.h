#pragma once
#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\hle\Memory\SystemThreadMemory.h>
#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <Common\Thread.h>
#include <Common\SyncEvent.h>
#include <stdint.h>
#include <map>

class CSwitchSystem;
class CHleKernel;

class CSystemThread :
    public CKernelObject,
    public CThread
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
        ThreadState_WaitEvent,
        ThreadState_WaitSleep,
    };

    CSystemThread(CSwitchSystem & System, CProcessMemory &ProcessMemory, const char * Name, uint64_t EntryPoint, uint32_t ThreadHandle, uint32_t ThreadId, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId, bool InitialThread);
    ~CSystemThread();

    inline uint32_t SystemThreadId() const { return m_ThreadId; }
    inline CSystemThreadMemory & ThreadMemory() { return m_ThreadMemory; }
    inline uint32_t GetPriority() const { return m_Priority; }
    inline ThreadState GetState() const { return m_State; }
    inline const char * GetName() const { return m_Name.c_str(); }
    inline const MutexWaitingThreads & GetWaitMutexThreads() const { return m_WaitMutexThreads; }
    inline uint64_t GetCondVarWaitAddress() const { return m_CondVarWaitAddress; }
    inline uint64_t GetMutexWaitAddress() const { return m_MutexWaitAddress; }
    inline CSystemThread * GetLockOwner() const { return m_LockOwner; }
    inline uint32_t GetWaitHandle() const { return m_WaitHandle; }
    inline uint64_t TlsAddress() const { return m_TlsAddress; }
    inline bool InitialThread() const { return m_InitialThread; }
    inline uint64_t & CpuTicks() { return m_CpuTicks; }
    inline const bool & ProcessEvents() const { return m_ProcessEvents; }

    int32_t WaitEvent(const KernelObjectList & EventObjects, int64_t Timeout);

    const char * GetStateName() const;
    IRegisters & Reg(void);
    void AddMutexWaiter(CSystemThread * thread);
    void RemoveMutexWaiter(CSystemThread * thread);
    void SetCondVarWaitAddress(uint64_t CondVarWaitAddress);
    void SetMutexWaitAddress(uint64_t MutexWaitAddress);
    void SetWaitHandle(uint32_t WaitHandle);
    void SetState(ThreadState state);
    void SetLockOwner(CSystemThread * LockThread);
    void SetProcessEvents(bool ProcessEvents);
    void Start(void);
    void ServiceCall(uint32_t index);
    void ResetSyncEvent(void);
    void TriggerSyncEvent(void);
    void WaitSyncEvent(int32_t iWaitTime);

private:
    CSystemThread();
    CSystemThread(const CSystemThread&);
    CSystemThread& operator=(const CSystemThread&);

    KernelObjectHandleType GetHandleType() const { return KernelObjectHandleType_Thread; }
    CSystemThread * GetSystemThreadPtr(void) { return this; }

    void EmulationThread(void);
    static uint32_t stEmulationThread(void* _this) { ((CSystemThread*)_this)->EmulationThread(); return 0; }
    
    CSwitchSystem & m_System;
    std::unique_ptr<CPUExecutor> m_CPU;
    CSystemThreadMemory m_ThreadMemory;
    SyncEvent m_SyncEvent;
    MutexWaitingThreads m_WaitMutexThreads;
    ThreadState m_State;
    uint32_t m_ThreadId;
	uint32_t m_Priority;
    uint64_t m_CondVarWaitAddress;
    uint64_t m_MutexWaitAddress;
    uint32_t m_WaitHandle;
    uint64_t m_TlsAddress;
    uint64_t m_CpuTicks;
    CSystemThread * m_LockOwner;
    bool m_ProcessEvents;
	std::string m_Name;
    bool m_InitialThread;
};
