#include "Engine/Maxwell3D.h"
#include "Renderer/Renderer.h"
#include "VideoMemoryManager.h"
#include "VideoNotification.h"

CMaxwell3D::CMaxwell3D(ISwitchSystem & SwitchSystem, CVideoMemory & VideoMemory) :
    m_SwitchSystem(SwitchSystem),
    m_VideoMemory(VideoMemory),
    m_MacroEngine(GetMacroEngine(*this)),
    m_ExecutingMacro(0),
    m_Renderer(nullptr),
    m_StateTracker(CMaxwell3D::NumRegisters, 0)
{
    memset(m_MacroPositions, 0, sizeof(m_MacroPositions));
    memset(&m_CBDataState, 0, sizeof(m_CBDataState));
    m_CBDataState.Current = 0xFFFFFFFF;
    m_CBDataState.Id = 0xFFFFFFFF;
    InitializeRegisterDefaults();
}

CMaxwell3D::~CMaxwell3D()
{
}

void CMaxwell3D::BindRenderer(IRenderer * Renderer)
{
    if (Renderer != nullptr && m_Renderer != nullptr) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_Renderer = Renderer;
}

void CMaxwell3D::InitializeRegisterDefaults()
{
    memset(&m_Regs, 0, sizeof(m_Regs));
    m_ShadowRegs = m_Regs;
}

void CMaxwell3D::ProcessMacro(uint32_t Method, const uint32_t * BaseStart, uint32_t Amount, bool Last)
{
    if (m_ExecutingMacro == 0)
    {
        if ((Method % 2) != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return;
        }
        m_ExecutingMacro = Method;
    }
    m_MacroParams.insert(m_MacroParams.end(), BaseStart, BaseStart + Amount);
    if (Last)
    {
        CallMacroMethod(m_ExecutingMacro, m_MacroParams);
        m_MacroParams.clear();
    }
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
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
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
        StartCBData(Method);
        break;
    case Method_CounterReset:
    case Method_DataUpload:
    case Method_DrawVertexEndGL:
    case Method_ExecUpload:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    case Method_Firmware4:
        ProcessFirmwareCall4();
        break;
    case Method_FragmentBarrier:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    case Method_MacrosBind:
        ProcessMacroBind(ShadowArgument);
        break;
    case Method_MacrosData:
        m_MacroEngine->AddCode(m_Regs.Macros.UploadAddress, ShadowArgument);
        break;
    case Method_QueryGet:
    case Method_ShadowRamControl:
    case Method_SyncInfo:
    case Method_TiledCacheBarrier:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    case Method_WaitForIdle:
        m_Renderer->WaitForIdle();
        break;
    }
}

void CMaxwell3D::CallMacroMethod(uint32_t Method, const MacroParams & Parameters)
{
    uint32_t Entry = ((Method - NumRegisters) >> 1) % (uint32_t)(sizeof(m_MacroPositions) / sizeof(m_MacroPositions[0]));
    m_ExecutingMacro = 0;
    m_MacroEngine->Execute(m_MacroPositions[Entry], Parameters);
}

void CMaxwell3D::CallMethod(Method Method, uint32_t Argument, bool Last)
{
    if (Method == m_CBDataState.Current)
    {
        m_Regs.Value[Method] = Argument;
        ProcessCBData(Argument);
        return;
    }
    else if (m_CBDataState.Current != 0xFFFFFFFF)
    {
        FinishCBData();
    }

    if (m_ExecutingMacro != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    if (Method >= NumRegisters)
    {
        ProcessMacro(Method, &Argument, 1, Last);
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
        ProcessMacro(Method, BaseStart, Amount, Amount == MethodsPending);
    }
    else if (Method >= Method_ConstBufferData0 && Method <= Method_ConstBufferData15) 
    {
        ProcessCBMultiData(Method, BaseStart, Amount);
    }
    else 
    {
        for (uint32_t i = 0; i < Amount; i++) 
        {
            CallMethod(Method, BaseStart[i], (int32_t)(MethodsPending - i) <= 1);
        }
    }
}

void CMaxwell3D::CallMethodFromMME(Method Method, uint32_t Argument)
{
    if (Method == Method_DrawVertexEndGL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else if (Method == Method_DrawVertexBeginGL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else if (Method == Method_VertexBufferCount || Method == Method_IndexArrayCount)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    else
    {
        CallMethod(Method, Argument, true);
    }
}

void CMaxwell3D::ProcessMacroBind(uint32_t Data)
{
    m_MacroPositions[m_Regs.Macros.Entry] = Data;
    m_Regs.Macros.Entry += 1;
}

void CMaxwell3D::ProcessFirmwareCall4()
{
    // stubbed by setting 0xd00 to 1.
    m_Regs.Value[0xd00] = 1;
}

void CMaxwell3D::ProcessCBData(uint32_t Value)
{
    m_CBDataState.Buffer[m_CBDataState.Id][m_CBDataState.Counter] = Value;
    m_CBDataState.Counter += 1;
    m_Regs.ConstBuffer.Pos = m_Regs.ConstBuffer.Pos + 4;
}

void CMaxwell3D::StartCBData(uint32_t Method)
{
    m_CBDataState.StartPos = m_Regs.ConstBuffer.Pos;
    m_CBDataState.Id = Method - Method_ConstBufferData0;
    m_CBDataState.Current = Method;
    m_CBDataState.Counter = 0;
    ProcessCBData(m_Regs.ConstBuffer.Data[m_CBDataState.Id]);
}

void CMaxwell3D::ProcessCBMultiData(uint32_t Method, const uint32_t * BaseStart, uint32_t Amount)
{
    if (m_CBDataState.Current != Method)
    {
        if (m_CBDataState.Current != 0xFFFFFFFF)
        {
            FinishCBData();
        }
        m_CBDataState.StartPos = m_Regs.ConstBuffer.Pos;
        m_CBDataState.Id = Method - Method_ConstBufferData0;
        m_CBDataState.Current = Method;
        m_CBDataState.Counter = 0;
    }
    uint32_t Id = m_CBDataState.Id;
    for (uint32_t i = 0, n = Amount; i < n; i++)
    {
        m_CBDataState.Buffer[Id][m_CBDataState.Counter] = BaseStart[i];
        m_CBDataState.Counter++;
    }
    m_Regs.ConstBuffer.Pos = m_Regs.ConstBuffer.Pos + 4 * Amount;
}

void CMaxwell3D::FinishCBData()
{
    if (m_Regs.ConstBuffer.Address() == 0 || m_Regs.ConstBuffer.Pos > m_Regs.ConstBuffer.Size)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    uint64_t Address = m_Regs.ConstBuffer.Address() + m_CBDataState.StartPos;
    uint32_t Size = m_Regs.ConstBuffer.Pos - m_CBDataState.StartPos;

    m_VideoMemory.WriteBuffer(Address, m_CBDataState.Buffer[m_CBDataState.Id], Size, true);
    m_StateTracker.OnMemoryWrite();

    m_CBDataState.Id = 0xFFFFFFFF;
    m_CBDataState.Current = 0xFFFFFFFF;
}

