#pragma once
#include <nxemu-core\hle\Memory\ProcessMemory.h>
#include <nxemu-core\hle\Kernel\HleKernel.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>
#include <nxemu-core\Machine\SwitchKeys.h>
#include <nxemu-core\Machine\SystemEvents.h>
#include <nxemu-core\FileSystem\EncryptedFile.h>
#include <nxemu-core\FileSystem\PartitionFilesystem.h>
#include <nxemu-core\Plugins\Plugins.h>
#include <nxemu-core\Plugins\VideoPlugin.h>
#include <Common\Thread.h>
#include <Common\path.h>
#include <Common\ThreadLocalStorage.h>

class CXci;

class CSwitchSystem  :
    public ISwitchSystem
{
    friend CSystemThread;

public:
    CSwitchSystem();
    ~CSwitchSystem();

    bool Initialize(IRenderWindow & Window);
    bool LoadGame(const char * GamePath);
    void StartEmulation(void);

    inline KernelObjectMap KernelObjects(void) { return m_Kernel.KernelObjects(); }
    inline CKernelObject * SystemThread(void) { return m_SystemThread.Get(); }
    inline CHleKernel & HleKernel(void) { return m_Kernel; }
    inline IVideo & Video(void) { return *m_Plugins.VideoPlugin(); }
    inline CSystemEvents & SystemEvents(void) { return m_SystemEvents; }
    inline CNvDriver & NvDriver(void) { return m_NvDriver; }
    inline CXci * Xci(void) const { return m_Xci; }
    inline bool & EndEmulation(void) { return m_EndEmulation; }

    //ISwitchSystem
    uint8_t * GetCPUMemoryPointer(uint64_t cpu_addr);
    const uint8_t * GetCPUMemoryPointer(uint64_t cpu_addr) const;
    bool ReadCPUMemory(uint64_t cpu_addr, void * DestBuffer, uint64_t Size) const;
    bool WriteCPUMemory(uint64_t cpu_addr, const void * SrcBuffer, uint64_t Size);
    void MarkRasterizerMemory(uint64_t CpuAddr, uint64_t size, bool cached);
    uint64_t TitleID(void);

private:
    CSwitchSystem(const CSwitchSystem&);
    CSwitchSystem& operator=(const CSwitchSystem&);

	bool LoadXCI(const CPath & XciFile);
	bool LoadNSOModule(uint64_t offset, CEncryptedFile &EncryptedFile, const CPartitionFilesystem::VirtualFile * file, uint64_t base_addr, uint64_t &end_addr);

    CSystemEvents m_SystemEvents;
    CNvDriver m_NvDriver;
    CHleKernel m_Kernel;
    CProcessMemory m_ProcessMemory;
    bool m_EndEmulation;
    CTLS<CKernelObject> m_SystemThread;
	CSwitchKeys m_Keys;
    CPlugins m_Plugins;
	CXci * m_Xci;
};