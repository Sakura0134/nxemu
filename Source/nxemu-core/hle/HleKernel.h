#pragma once
#include <nxemu-core\hle\ProcessMemory.h>
#include <nxemu-core\hle\SystemThread.h>

class CSwitchSystem;

class CHleKernel
{
public:   
    CHleKernel(CSwitchSystem & System, CProcessMemory & ProcessMemory);
    ~CHleKernel();

	bool AddSystemThread(uint32_t & ThreadHandle, const char * name, uint64_t entry_point, uint64_t ThreadContext, uint64_t StackTop, uint32_t Priority, uint32_t ProcessorId);
    const SystemThreadList & SystemThreads(void) const { return m_SystemThreads; }

private:
    CHleKernel(void);                          // Disable default constructor
    CHleKernel(const CHleKernel&);             // Disable copy constructor
    CHleKernel& operator=(const CHleKernel&);  // Disable assignment

	uint32_t GetNewHandle();
	uint64_t CreateNewThreadID();

    CSwitchSystem & m_System;
    CProcessMemory & m_ProcessMemory;
    SystemThreadList m_SystemThreads;
    uint32_t m_NextHandle;
	uint64_t m_NextThreadId;
};