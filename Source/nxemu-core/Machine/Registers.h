#pragma once
#include <Common\stdtypes.h>

class CPUExecutor;

class CRegisters
{
    friend CPUExecutor;

public:
    CRegisters(CPUExecutor * Executor);

private:
    CRegisters(void);                         // Disable default constructor
    CRegisters(const CRegisters&);            // Disable copy constructor
    CRegisters& operator=(const CRegisters&); // Disable assignment

    CPUExecutor * m_Executor;
};

