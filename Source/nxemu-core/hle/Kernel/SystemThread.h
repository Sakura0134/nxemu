#pragma once
#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\hle\Memory\SystemThreadMemory.h>
#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <Common\stdtypes.h>
#include <map>

class CHleKernel;

class CSystemThread :
    public CPUExecutor,
    public CKernelObject
{
public:
    enum ThreadState
    {
        Unknown,
        Created,
        Ready,
        Running,
    };

    CSystemThread(CHleKernel * m_Kernel, CProcessMemory &ProcessMemory, const char * name, uint64_t entry_point, uint32_t ThreadHandle, uint32_t thread_id, uint64_t ThreadContext, uint64_t StackTop, uint32_t StackSize, uint32_t Priority, uint32_t ProcessorId);
    ~CSystemThread();

    inline ThreadState GetState() const { return m_State; }

    void SetState(ThreadState state);

private:
    CSystemThread();                               // Disable default constructor
    CSystemThread(const CSystemThread&);            // Disable copy constructor
    CSystemThread& operator=(const CSystemThread&); // Disable assignment

    HandleType GetHandleType() const { return Thread; }
    CSystemThread * GetSystemThreadPtr(void) { return this; }

    void ServiceCall(uint32_t index);
        
    CHleKernel * m_Kernel;
    CSystemThreadMemory m_ThreadMemory;
    ThreadState m_State;
    uint32_t m_ThreadId;
	uint32_t m_Priority;
    uint64_t m_TlsAddress;
	std::string m_Name;
};

typedef std::map<uint32_t, CSystemThread*> SystemThreadList;