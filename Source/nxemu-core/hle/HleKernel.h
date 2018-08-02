#pragma once
#include <nxemu-core\hle\SystemThread.h>

class CSwitchSystem;

class CHleKernel
{
public:   
    CHleKernel(CSwitchSystem & System);
    ~CHleKernel();

    const SystemThreadList & SystemThreads(void) const { return m_SystemThreads; }

private:
    CHleKernel(void);                          // Disable default constructor
    CHleKernel(const CHleKernel&);             // Disable copy constructor
    CHleKernel& operator=(const CHleKernel&);  // Disable assignment

    CSwitchSystem & m_System;
    SystemThreadList m_SystemThreads;
};