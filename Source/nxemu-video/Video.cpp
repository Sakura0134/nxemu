#include "Video.h"
#include "VideoNotification.h"
#include "Textures\Decoder.h"
#include "Shader\ShaderOpCode.h"
#include "Framebuffer.h"
#include <stdio.h>

CVideo::CVideo(IRenderWindow & RenderWindow, ISwitchSystem & SwitchSystem) :
    m_Memory(SwitchSystem),
    m_Maxwell3D(SwitchSystem, m_Memory),
    m_GpuThread(SwitchSystem, *this),
    m_EmulatorWindow(RenderWindow),
    m_SyncPointEvent(true)
{
    memset(m_BoundEngines, 0, sizeof(m_BoundEngines));
    memset(m_Regs.Value, 0, sizeof(m_Regs.Value));
    memset(m_SyncPoints, 0, sizeof(m_SyncPoints));
}

CVideo::~CVideo()
{
}

bool CVideo::Initialize(void)
{
    InitSwizzleTable();
    ShaderOpCode::CreateDecodeTable();
    if (!m_GpuThread.StartThread())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    return true;
}

void CVideo::PushGPUEntries(const uint64_t * Entries, uint32_t NoOfEntries)
{
    m_GpuThread.PushCommands(Entries, NoOfEntries);
}

void CVideo::SwapBuffers(uint64_t Address, uint32_t Offset, uint32_t Format, uint32_t Width, uint32_t Height, uint32_t Stride, uint32_t Transform, int32_t CropLeft, int32_t CropTop, int32_t CropRight, int32_t CropBottom)
{
    m_GpuThread.SwapBuffers(CFramebuffer(Address, Offset,(GPUPixelFormat)Format, Width, Height, Stride, (TransformFlags)Transform, CropLeft, CropTop, CropRight, CropBottom));
}

uint32_t CVideo::GetSyncPointValue(uint32_t SyncPointId) const
{
    if (SyncPointId >= MaxSyncPoints)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    uint32_t SyncPointValue;
    {
        CGuard Guard(m_CS);
        SyncPointValue = m_SyncPoints[SyncPointId];
    }
    return SyncPointValue;
}

void CVideo::WaitFence(uint32_t SyncPointId, uint32_t Value)
{
    if (SyncPointId == UINT32_MAX)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    uint32_t SyncPointValue;
    for (;;) 
    {
        {
            CGuard Guard(m_CS);
            SyncPointValue = m_SyncPoints[SyncPointId];
            m_SyncPointEvent.Reset();
        }
        if (SyncPointValue >= Value)
        {
            break;
        }
        m_SyncPointEvent.IsTriggered(SyncEvent::INFINITE_TIMEOUT);
    }
}

void CVideo::InvalidateRegion(uint64_t CpuAddr, uint32_t Size)
{
    m_GpuThread.FlushAndInvalidateRegion(CpuAddr, Size);
}

void CVideo::FlushRegion(uint64_t /*CpuAddr*/, uint64_t /*size*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

uint64_t CVideo::VideoMemoryAllocate(uint64_t Size, uint64_t Align)
{
    CGuard Guard(m_CS);
    return m_Memory.Allocate(Size, Align);
}

uint64_t CVideo::VideoMemoryAllocateFixed(uint64_t /*GpuAddr*/, uint64_t /*Size*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

void CVideo::VideoMemoryMap(uint64_t CpuAddr, uint64_t GpuAddr, uint64_t size)
{
    CGuard Guard(m_CS);
    m_Memory.Map(CpuAddr, GpuAddr, size);
}

uint64_t CVideo::VideoMemoryMapAllocate(uint64_t CpuAddr, uint64_t Size, uint64_t Align)
{
    CGuard Guard(m_CS);
    return m_Memory.MapAllocate(CpuAddr, Size, Align);
}

void CVideo::BindRenderer(IRenderer * Renderer) 
{
    m_Memory.BindRenderer(Renderer);
    m_Maxwell3D.BindRenderer(Renderer);
}

void CVideo::IncrementSyncPoint(const uint32_t SyncPointId) 
{
    uint32_t SyncPointValue;
    {
        CGuard Guard(m_CS);
        m_SyncPoints[SyncPointId] += 1;
        SyncPointValue = m_SyncPoints[SyncPointId];
        m_SyncPointEvent.Trigger();
    }
}

void CVideo::CallMethod(BufferMethods Method, uint32_t Argument, uint32_t SubChannel, uint32_t MethodCount) 
{
    if (Method >= BufferMethods_NonPuller) 
    {
        bool LastCall = MethodCount <= 1;

        if (SubChannel >= sizeof(m_BoundEngines)/sizeof(m_BoundEngines[0]))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        switch (m_BoundEngines[SubChannel])
        {
        case EngineID_FERMI_TWOD_A:
            m_Fermi2D.CallMethod(Method, Argument, LastCall);
            break;
        case EngineID_MAXWELL_B:
            m_Maxwell3D.CallMethod((CMaxwell3D::Method)Method, Argument, LastCall);
            break;
        case EngineID_KEPLER_COMPUTE_B:
            m_KeplerCompute.CallMethod(Method, Argument, LastCall);
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else 
    {
        CallPullerMethod(Method, Argument, SubChannel);
    }
}

void CVideo::CallMultiMethod(uint32_t Method, uint32_t SubChannel, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending)
{
    if (SubChannel >= sizeof(m_BoundEngines) / sizeof(m_BoundEngines[0]))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    if (Method >= BufferMethods_NonPuller) 
    {
        CallEngineMultiMethod(Method, SubChannel, BaseStart, Amount, MethodsPending);
    } 
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CVideo::CallPullerMethod(BufferMethods Method, uint32_t Argument, uint32_t SubChannel) 
{
    if (Method >= Registers::NUM_REGS)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_Regs.Value[Method] = Argument;

    switch (Method) 
    {
    case BufferMethods_BindObject: 
        if (SubChannel >= (sizeof(m_BoundEngines) / sizeof(m_BoundEngines[0])))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        m_BoundEngines[SubChannel] = (EngineID)Argument;
        break;
    case BufferMethods_Nop:
    case BufferMethods_SemaphoreAddressHigh:
    case BufferMethods_SemaphoreAddressLow:
    case BufferMethods_SemaphoreSequence:
    case BufferMethods_RefCnt:
    case BufferMethods_UnkCacheFlush:
    case BufferMethods_WrcacheFlush:
    case BufferMethods_FenceValue:
        break;
    case BufferMethods_FenceAction:
        ProcessFenceActionMethod();
        break;
    case BufferMethods_Yield:
    case BufferMethods_WaitForInterrupt:
    case BufferMethods_NotifyIntr:
    case BufferMethods_Unk28:
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
}

void CVideo::CallEngineMultiMethod(uint32_t Method, uint32_t SubChannel, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending)
{
    switch (m_BoundEngines[SubChannel]) 
    {
    case EngineID_MAXWELL_B:
        m_Maxwell3D.CallMultiMethod((CMaxwell3D::Method)Method, BaseStart, Amount, MethodsPending);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CVideo::ProcessFenceActionMethod() 
{
    switch (m_Regs.FenceAction.Op) 
    {
    case FenceOperation_Acquire:
        WaitFence(m_Regs.FenceAction.SyncPointId, m_Regs.FenceValue);
        break;
    case FenceOperation_Increment:
        IncrementSyncPoint(m_Regs.FenceAction.SyncPointId);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CVideo::OnCommandListEnd() 
{
    m_GpuThread.OnCommandListEnd();
}
