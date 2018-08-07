#pragma once
#include <nxemu-core\Machine\Registers.h>

class CPUExecutor
{    
public:
    CPUExecutor(void);

    void Execute(bool & Done);

protected:
    CRegisters m_Reg;
private:
    CPUExecutor(const CPUExecutor&);            // Disable copy constructor
    CPUExecutor& operator=(const CPUExecutor&); // Disable assignment
};
