#pragma once
#include <Common\Thread.h>
#include <Common\path.h>
#include <nxemu-core\hle\ProcessMemory.h>
#include <nxemu-core\hle\HleKernel.h>
#include <nxemu-core\hle\SystemThread.h>
#include <nxemu-core\Machine\SwitchKeys.h>

class CSwitchSystem 
{
public:
    CSwitchSystem();
    ~CSwitchSystem();

    bool LoadGame(const char * GamePath);
    void StartEmulation(void);

private:
    CSwitchSystem(const CSwitchSystem&);             // Disable copy constructor
    CSwitchSystem& operator=(const CSwitchSystem&);	 // Disable assignment

	bool LoadXCI(const CPath & XciFile);
    bool LoadNsoFile(const CPath & NsoFile, uint64_t base_addr, uint64_t &end_addr);
    void EmulationThread(void);

    static uint32_t stEmulationThread(void * _this) { ((CSwitchSystem*)_this)->EmulationThread(); return 0; }

    CHleKernel m_Kernel;
    CProcessMemory m_ProcessMemory;
    bool m_EndEmulation;
    CThread m_EmulationThread;
	CSwitchKeys m_Keys;
};