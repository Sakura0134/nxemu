#pragma once
#include <nxemu-core\Settings\Debug.h>
#include <nxemu-core\Machine\Registers.h>
#include <nxemu-core\Machine\CPU\MemoryManagement.h>

class CPUExecutor :
    protected CDebugSettings
{    
public:
    CPUExecutor(MemoryManagement & mmu);

    void Execute(bool & Done);

    CRegisters & Reg(void) { return m_Reg; }
    MemoryManagement & MMU(void) { return m_MMU; }

protected:
    CRegisters m_Reg;
    MemoryManagement & m_MMU;

private:
    CPUExecutor(void);                          // Disable default constructor
    CPUExecutor(const CPUExecutor&);            // Disable copy constructor
    CPUExecutor& operator=(const CPUExecutor&); // Disable assignment
};
