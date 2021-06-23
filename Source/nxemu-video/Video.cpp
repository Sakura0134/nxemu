#include "Video.h"
#include "VideoNotification.h"
#include <stdio.h>

CVideo::CVideo(IRenderWindow & RenderWindow, ISwitchSystem & SwitchSystem) :
    m_RenderWindow(RenderWindow),
    m_Memory(SwitchSystem),
    m_Maxwell3D(SwitchSystem, m_Memory),
    m_GpuThread(SwitchSystem, *this)
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

void CVideo::SwapBuffers(uint64_t /*Address*/, uint32_t /*Offset*/, uint32_t /*Format*/, uint32_t /*Width*/, uint32_t /*Height*/, uint32_t /*Stride*/, uint32_t /*Transform*/, int32_t /*CropLeft*/, int32_t /*CropTop*/, int32_t /*CropRight*/, int32_t /*CropBottom*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
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

void CVideo::WaitFence(uint32_t /*SyncPointId*/, uint32_t /*value*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CVideo::InvalidateRegion(uint64_t /*CpuAddr*/, uint64_t /*size*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
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

