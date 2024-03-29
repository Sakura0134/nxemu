#pragma once
#include "VideoMemoryManager.h"
#include "GpuThread.h"
#include "GpuTypes.h"
#include "Engine\Maxwell3D.h"
#include "Engine\KeplerCompute.h"
#include "Engine\Fermi2D.h"
#include "EmulatorWindow.h"
#include <nxemu-plugin-spec\Video.h>
#include <Common\CriticalSection.h>
#include <Common\Padding.h>

class CVideo :
    public IVideo
{
public:
    enum
    {
        MaxSyncPoints = 192,
    };

    enum FenceOperation : unsigned
    {
        FenceOperation_Acquire = 0,
        FenceOperation_Increment = 1,
    };

#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
    union tyFenceAction 
    {
        uint32_t Value;
        struct 
        {
            FenceOperation Op : 1;
            unsigned : 7;
            unsigned SyncPointId : 24;
        };
    };

    union Registers 
    {
        enum { NUM_REGS = 0x40 };
        struct 
        {
            PADDING_WORDS(0x1C);
            uint32_t FenceValue;
            tyFenceAction FenceAction;
        };
        uint32_t Value[NUM_REGS];
    };
#pragma warning(pop)

    CVideo(IRenderWindow & RenderWindow, ISwitchSystem & SwitchSystem);
    ~CVideo();

    //IVideo
    bool Initialize(void);
    void PushGPUEntries(const uint64_t* Entries, uint32_t NoOfEntries);
    void SwapBuffers(uint64_t Address, uint32_t Offset, uint32_t Format, uint32_t Width, uint32_t Height, uint32_t Stride, uint32_t Transform, int32_t CropLeft, int32_t CropTop, int32_t CropRight, int32_t CropBottom);
    uint32_t GetSyncPointValue(uint32_t SyncPointId) const;
    void WaitFence(uint32_t SyncPointId, uint32_t value);
    void FlushRegion(uint64_t CpuAddr, uint64_t size);
    void InvalidateRegion(uint64_t Addr, uint32_t Size);
    uint64_t VideoMemoryAllocate(uint64_t Size, uint64_t Align);
    uint64_t VideoMemoryAllocateFixed(uint64_t GpuAddr, uint64_t Size);
    void VideoMemoryMap(uint64_t CpuAddr, uint64_t GpuAddr, uint64_t size);
    uint64_t VideoMemoryMapAllocate(uint64_t CpuAddr, uint64_t size, uint64_t align);

    void BindRenderer(IRenderer * Renderer);
    void CallMethod(BufferMethods Method, uint32_t Argument, uint32_t SubChannel, uint32_t MethodCount);
    void CallMultiMethod(uint32_t Method, uint32_t SubChannel, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending);
    void OnCommandListEnd();
    void IncrementSyncPoint(uint32_t SyncPointId);

    CVideoMemory & VideoMemory() { return m_Memory; }
    EmulatorWindow & Window() { return m_EmulatorWindow; }
    CMaxwell3D & Maxwell3D() { return m_Maxwell3D; }

private:
    CVideo();
    CVideo(const CVideo&);
    CVideo& operator=(const CVideo&);

    void ProcessFenceActionMethod();
    void CallEngineMultiMethod(uint32_t Method, uint32_t SubChannel, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending);
    void CallPullerMethod(BufferMethods Method, uint32_t Argument, uint32_t SubChannel);

    CVideoMemory m_Memory;
    EngineID m_BoundEngines[8];
    CMaxwell3D m_Maxwell3D;
    CFermi2D m_Fermi2D;
    CKeplerCompute m_KeplerCompute;
    Registers m_Regs;
    uint32_t m_SyncPoints[MaxSyncPoints];
    SyncEvent m_SyncPointEvent;
    mutable CriticalSection m_CS;
    CGpuThread m_GpuThread;
    EmulatorWindow m_EmulatorWindow;
};

#define ASSERT_REG_POSITION(field_name, position) static_assert(offsetof(CVideo::Registers, field_name) == position * 4, "Field " #field_name " has invalid position")

ASSERT_REG_POSITION(FenceValue, 0x1C);
ASSERT_REG_POSITION(FenceAction, 0x1D);

#undef ASSERT_REG_POSITION
