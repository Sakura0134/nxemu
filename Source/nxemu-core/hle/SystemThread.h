#pragma once
#include <map>
#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\hle\ProcessMemory.h>

class CHleKernel;

class CSystemThread :
    public CPUExecutor
{
public:
    CSystemThread(CHleKernel * m_Kernel, CProcessMemory &ProcessMemory, const char * name, uint64_t entry_point, uint32_t ThreadHandle, uint32_t thread_id, uint64_t ThreadContext, uint64_t StackTop, uint32_t Priority, uint32_t ProcessorId);
    ~CSystemThread();

private:
    CSystemThread();                               // Disable default constructor
    CSystemThread(const CSystemThread&);            // Disable copy constructor
    CSystemThread& operator=(const CSystemThread&); // Disable assignment
        
    CHleKernel * m_Kernel;
    uint32_t m_thread_id;
	uint32_t m_Priority;
	std::string m_Name;
};

typedef std::map<uint32_t, CSystemThread*> SystemThreadList;