#pragma once
#include <Common\path.h>
#include <nxemu-core\hle\ProcessMemory.h>

class CSwitchSystem 
{
public:
    CSwitchSystem();
    ~CSwitchSystem();

    bool LoadGameDir(const char * GameDir);

private:
    CSwitchSystem(const CSwitchSystem&);             // Disable copy constructor
    CSwitchSystem& operator=(const CSwitchSystem&);	 // Disable assignment

    bool LoadNsoFile(const CPath & NsoFile, uint64_t base_addr, uint64_t &end_addr);

    CProcessMemory m_ProcessMemory;
};