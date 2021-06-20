#include "Engine/Maxwell3D.h"
#include "VideoNotification.h"

CMaxwell3D::CMaxwell3D(ISwitchSystem & SwitchSystem, CVideoMemory & VideoMemory) :
    m_SwitchSystem(SwitchSystem),
    m_VideoMemory(VideoMemory),
    m_StateTracker(CMaxwell3D::NumRegisters, 0)
{
    memset(m_MacroPositions, 0, sizeof(m_MacroPositions));
    InitializeRegisterDefaults();
}

CMaxwell3D::~CMaxwell3D()
{
}

void CMaxwell3D::InitializeRegisterDefaults()
{
    memset(&m_Regs, 0, sizeof(m_Regs));
    m_ShadowRegs = m_Regs;
}

uint32_t CMaxwell3D::ProcessShadowRam(uint32_t Method, uint32_t Argument)
{
    ShadowRamControl Control = m_ShadowRegs.ShadowRamControl;
    if (Control == ShadowRamControl_Replay)
    {
        return m_ShadowRegs.Value[Method];
    }
    if (Control == ShadowRamControl_Track || Control == ShadowRamControl_TrackWithFilter)
    {
        m_ShadowRegs.Value[Method] = Argument;
    }
    return Argument;
}

void CMaxwell3D::ProcessMethodCall(Method Method, uint32_t ShadowArgument, uint32_t /*Argument*/, bool /*Last*/)
{
    switch (Method)
    {
    case Method_CBBind0:
    case Method_CBBind1:
    case Method_CBBind2:
    case Method_CBBind3:
    case Method_CBBind4:
    case Method_ClearBuffers:
    case Method_ConditionMode:
    case Method_ConstBufferData0:
    case Method_ConstBufferData1:
    case Method_ConstBufferData2:
    case Method_ConstBufferData3:
    case Method_ConstBufferData4:
    case Method_ConstBufferData5:
    case Method_ConstBufferData6:
    case Method_ConstBufferData7:
    case Method_ConstBufferData8:
    case Method_ConstBufferData9:
    case Method_ConstBufferData10:
    case Method_ConstBufferData11:
    case Method_ConstBufferData12:
    case Method_ConstBufferData13:
    case Method_ConstBufferData14:
    case Method_ConstBufferData15:
    case Method_CounterReset:
    case Method_DataUpload:
    case Method_DrawVertexEndGL:
    case Method_ExecUpload:
    case Method_Firmware4:
    case Method_FragmentBarrier:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    case Method_MacrosBind:
        ProcessMacroBind(ShadowArgument);
        break;
    case Method_MacrosData:
    case Method_QueryGet:
    case Method_ShadowRamControl:
    case Method_SyncInfo:
    case Method_TiledCacheBarrier:
    case Method_WaitForIdle:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void CMaxwell3D::CallMethod(Method Method, uint32_t Argument, bool Last)
{
    if (Method >= NumRegisters)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else
    {
        uint32_t ShadowArgument = ProcessShadowRam(Method, Argument);
        if (m_Regs.Value[Method] != ShadowArgument)
        {
            m_Regs.Value[Method] = ShadowArgument;
            m_StateTracker.RegisterChanged(Method);
        }
        ProcessMethodCall(Method, ShadowArgument, Argument, Last);
    }
}

void CMaxwell3D::CallMultiMethod(Method Method, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending) 
{
    if (Method >= NumRegisters)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else if (Method >= Method_ConstBufferData0 && Method <= Method_ConstBufferData15) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else 
    {
        for (uint32_t i = 0; i < Amount; i++) 
        {
            CallMethod(Method, BaseStart[i], (int32_t)(MethodsPending - i) <= 1);
        }
    }
}

void CMaxwell3D::ProcessMacroBind(uint32_t Data)
{
    m_MacroPositions[m_Regs.Macros.Entry] = Data;
    m_Regs.Macros.Entry += 1;
}
