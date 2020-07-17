#pragma once
#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\hle\Kernel\HleKernel.h>
#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\Machine\SwitchKeys.h>
#include <nxemu-core\FileSystem\EncryptedFile.h>
#include <nxemu-core\FileSystem\PartitionFilesystem.h>
#include <Common\Thread.h>
#include <Common\path.h>
#include <Common\ThreadLocalStorage.h>

class CXci;

class CSwitchSystem 
{
public:
    CSwitchSystem();
    ~CSwitchSystem();

    bool LoadGame(const char * GamePath);
    void StartEmulation(void);

    inline KernelObjectMap & KernelObjects(void) { return m_Kernel.KernelObjects(); }
    inline CKernelObject * SystemThread(void) { return m_SystemThread.Get(); }
    inline CHleKernel & HleKernel(void) { return m_Kernel; }
    inline CXci * Xci(void) const { return m_Xci; }

private:
    CSwitchSystem(const CSwitchSystem&);             // Disable copy constructor
    CSwitchSystem& operator=(const CSwitchSystem&);	 // Disable assignment

	bool LoadXCI(const CPath & XciFile);
	bool LoadNSOModule(uint64_t offset, CEncryptedFile &EncryptedFile, const CPartitionFilesystem::VirtualFile * file, uint64_t base_addr, uint64_t &end_addr);
    bool LoadNsoFile(const CPath & NsoFile, uint64_t base_addr, uint64_t &end_addr);
    void EmulationThread(void);

    static uint32_t stEmulationThread(void * _this) { ((CSwitchSystem*)_this)->EmulationThread(); return 0; }

    CHleKernel m_Kernel;
    CProcessMemory m_ProcessMemory;
    bool m_EndEmulation;
    CThread m_EmulationThread;
    CTLS<CKernelObject> m_SystemThread;
	CSwitchKeys m_Keys;
	CXci * m_Xci;
};