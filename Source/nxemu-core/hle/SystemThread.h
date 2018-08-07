#pragma once
#include <map>
#include <nxemu-core\Machine\CPU\CPUExecutor.h>
#include <nxemu-core\hle\ProcessMemory.h>

class CHleKernel;

class CSystemThread :
    public CPUExecutor
{
public:
    CSystemThread(CHleKernel * m_Kernel, CProcessMemory &ProcessMemory, const char * name, uint64_t entry_point, uint32_t thread_id);
    ~CSystemThread();

private:
    CSystemThread();                               // Disable default constructor
    CSystemThread(const CSystemThread&);            // Disable copy constructor
    CSystemThread& operator=(const CSystemThread&); // Disable assignment
        
    CHleKernel * m_Kernel;
    uint32_t m_thread_id;
};

typedef std::map<uint32_t, CSystemThread*> SystemThreadList;