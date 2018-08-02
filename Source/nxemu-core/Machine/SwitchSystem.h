#pragma once
#include <Common\Thread.h>
#include <Common\path.h>
#include <nxemu-core\hle\ProcessMemory.h>

class CSwitchSystem 
{
public:
    CSwitchSystem();
    ~CSwitchSystem();

    bool Initialize(void);
    bool LoadGameDir(const char * GameDir);
    void StartEmulation(void);

private:
    CSwitchSystem(const CSwitchSystem&);             // Disable copy constructor
    CSwitchSystem& operator=(const CSwitchSystem&);	 // Disable assignment

    bool LoadNsoFile(const CPath & NsoFile, uint64_t base_addr, uint64_t &end_addr);
    void EmulationThread(void);

    static uint32_t stEmulationThread(void * _this) { ((CSwitchSystem*)_this)->EmulationThread(); return 0; }

    CProcessMemory m_ProcessMemory;
    bool m_EndEmulation;
    CThread m_EmulationThread;
};