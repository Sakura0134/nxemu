#pragma once

class CPUExecutor
{    
public:
    CPUExecutor(void);

    void Execute(bool & Done);

private:
    CPUExecutor(const CPUExecutor&);            // Disable copy constructor
    CPUExecutor& operator=(const CPUExecutor&); // Disable assignment
};
