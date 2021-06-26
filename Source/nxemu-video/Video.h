#pragma once
#include "VideoMemoryManager.h"
#include "GpuThread.h"
#include "GpuTypes.h"
#include "Engine\Maxwell3D.h"
#include "Engine\Fermi2D.h"
#include "EmulatorWindow.h"
#include <nxemu-plugin-spec\Video.h>
#include <Common\CriticalSection.h>
#include <Common\Padding.h>

class CVideo :
    public IVideo
{
    enum 
    {
        MaxSyncPoints = 192,
    };
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
    union Registers 
    {
        enum { NUM_REGS = 0x40 };
        struct 
        {
            PADDING_WORDS(0x40);
        };
        uint32_t Value[NUM_REGS];
    };
#pragma warning(pop)

public:
    CVideo(IRenderWindow & RenderWindow, ISwitchSystem & SwitchSystem);
    ~CVideo();

    //IVideo
    bool Initialize(void);
    void PushGPUEntries(const uint64_t* Entries, uint32_t NoOfEntries);
    void SwapBuffers(uint64_t Address, uint32_t Offset, uint32_t Format, uint32_t Width, uint32_t Height, uint32_t Stride, uint32_t Transform, int32_t CropLeft, int32_t CropTop, int32_t CropRight, int32_t CropBottom);
    uint32_t GetSyncPointValue(uint32_t SyncPointId) const;
    void WaitFence(uint32_t SyncPointId, uint32_t value);
    void FlushRegion(uint64_t CpuAddr, uint64_t size);
    void InvalidateRegion(uint64_t CpuAddr, uint64_t size);
    uint64_t VideoMemoryAllocate(uint64_t Size, uint64_t Align);
    uint64_t VideoMemoryAllocateFixed(uint64_t GpuAddr, uint64_t Size);
    void VideoMemoryMap(uint64_t CpuAddr, uint64_t GpuAddr, uint64_t size);
    uint64_t VideoMemoryMapAllocate(uint64_t CpuAddr, uint64_t size, uint64_t align);

    void BindRenderer(IRenderer * Renderer);
    void CallMethod(BufferMethods Method, uint32_t Argument, uint32_t SubChannel, uint32_t MethodCount);
    void CallMultiMethod(uint32_t Method, uint32_t SubChannel, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending);

    CVideoMemory & VideoMemory() { return m_Memory; }
    EmulatorWindow & Window() { return m_EmulatorWindow; }

private:
    CVideo();
    CVideo(const CVideo&);
    CVideo& operator=(const CVideo&);

    void CallEngineMultiMethod(uint32_t Method, uint32_t SubChannel, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending);
    void CallPullerMethod(BufferMethods Method, uint32_t Argument, uint32_t SubChannel);

    CVideoMemory m_Memory;
    EngineID m_BoundEngines[8];
    CMaxwell3D m_Maxwell3D;
    CFermi2D m_Fermi2D;
    Registers m_Regs;
    uint32_t m_SyncPoints[MaxSyncPoints];
    mutable CriticalSection m_CS;
    CGpuThread m_GpuThread;
    EmulatorWindow m_EmulatorWindow;
};