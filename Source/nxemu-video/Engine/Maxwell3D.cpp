#include "Engine/Maxwell3D.h"
#include "Renderer/Renderer.h"
#include "VideoMemoryManager.h"
#include "VideoNotification.h"

CMaxwell3D::CMaxwell3D(ISwitchSystem & SwitchSystem, CVideoMemory & VideoMemory) :
    m_SwitchSystem(SwitchSystem),
    m_VideoMemory(VideoMemory),
    m_MacroEngine(GetMacroEngine(*this)),
    m_UploadState(VideoMemory, m_Regs.Upload),
    m_ExecutingMacro(0),
    m_Renderer(nullptr),
    m_StateTracker(CMaxwell3D::NumRegisters, 0)
{
    memset(m_MacroPositions, 0, sizeof(m_MacroPositions));
    memset(&m_CBDataState, 0, sizeof(m_CBDataState));
    memset(&m_ShaderStage, 0, sizeof(m_ShaderStage));
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

    m_Regs.StencilFrontOpFail = StencilOp_Keep;
    m_Regs.StencilFrontOpZFail = StencilOp_Keep;
    m_Regs.StencilFrontOpZPass = StencilOp_Keep;
    m_Regs.StencilFrontFuncFunc = ComparisonOp_Always;
    m_Regs.StencilFrontFuncMask = 0xFFFFFFFF;
    m_Regs.StencilFrontMask = 0xFFFFFFFF;
    m_Regs.StencilTwoSideEnable = 1;
    m_Regs.StencilBackOpFail = StencilOp_Keep;
    m_Regs.StencilBackOpZFail = StencilOp_Keep;
    m_Regs.StencilBackOpZPass = StencilOp_Keep;
    m_Regs.StencilBackFuncFunc = ComparisonOp_Always;
    m_Regs.StencilBackFuncMask = 0xFFFFFFFF;
    m_Regs.StencilBackMask = 0xFFFFFFFF;

    for (uint32_t i = 0, n = sizeof(m_Regs.ColorMask) / sizeof(m_Regs.ColorMask[0]); i < n; i++)
    {
        tyColorMask & ColorMask = m_Regs.ColorMask[i];
        ColorMask.R = 1;
        ColorMask.G = 1;
        ColorMask.B = 1;
        ColorMask.A = 1;
    }

    m_Regs.RasterizeEnable = 1;

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

void CMaxwell3D::ProcessMethodCall(Method Method, uint32_t ShadowArgument, uint32_t Argument, bool Last)
{
    switch (Method)
    {
    case Method_CBBind0:
        ProcessCBBind(0);
        break;
    case Method_CBBind1:
        ProcessCBBind(1);
        break;
    case Method_CBBind2:
        ProcessCBBind(2);
        break;
    case Method_CBBind3:
        ProcessCBBind(3);
        break;
    case Method_CBBind4:
        ProcessCBBind(4);
        break;
    case Method_ClearBuffers:
        ProcessClearBuffers();
        break;
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
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    case Method_DataUpload:
        m_UploadState.ProcessData(ShadowArgument, Last);
        if (Last)
        {
            m_StateTracker.OnMemoryWrite();
        }
        break;
    case Method_DrawVertexEndGL:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    case Method_ExecUpload:
        m_UploadState.ProcessExec(m_Regs.ExecUpload.Linear != 0);
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
        ProcessQueryGet();
        break;
    case Method_ShadowRamControl:
        m_ShadowRegs.ShadowRamControl = (ShadowRamControl)Argument;
        break;
    case Method_SyncInfo:
        ProcessSyncPoint();
        break;
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

void CMaxwell3D::ProcessQueryGet()
{
    if (m_Regs.Query.QueryGet.Unit != QueryUnit_Crop)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    switch (m_Regs.Query.QueryGet.Operation)
    {
    case QueryOperation_Release:
        if (m_Regs.Query.QueryGet.Fence == 1)
        {
            m_Renderer->SignalSemaphore(m_Regs.Query.Address(), m_Regs.Query.QuerySequence);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
}

void CMaxwell3D::ProcessSyncPoint()
{
    if (m_Regs.SyncInfo.Increment)
    {
        m_Renderer->SignalSyncPoint(m_Regs.SyncInfo.SyncPoint);
    }
}

void CMaxwell3D::ProcessCBBind(uint32_t StageIndex)
{
    tyCBBind & BindData = m_Regs.CBBind[StageIndex];
    if (BindData.Index >= MaxConstBuffers)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    tyShaderStage & Buffer = m_ShaderStage[StageIndex][BindData.Index];
    Buffer.Enabled = BindData.Valid != 0;
    Buffer.Address = m_Regs.ConstBuffer.Address();
    Buffer.Size = m_Regs.ConstBuffer.Size;
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

void CMaxwell3D::ProcessClearBuffers()
{
    m_Renderer->Clear();
}

uint64_t CMaxwell3D::_tyConstBuffer::Address() const
{
    return (((uint64_t)AddressHigh) << 32) | AddressLow;
}

uint64_t CMaxwell3D::_tyQuery::Address() const
{
    return (((uint64_t)AddressHigh) << 32) | AddressLow;
}

uint64_t CMaxwell3D::_tyRenderTarget::Address() const 
{
    return (((uint64_t)AddressHigh) << 32) | AddressLow;
}

uint32_t CMaxwell3D::_tyRTControl::Map(uint32_t Index) const
{
    switch (Index)
    {
    case 0: return Map0;
    case 1: return Map1;
    case 2: return Map2;
    case 3: return Map3;
    case 4: return Map4;
    case 5: return Map5;
    case 6: return Map6;
    case 7: return Map7;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

uint64_t CMaxwell3D::_tyZeta::Address() const
{
    return (((uint64_t)AddressHigh) << 32) | AddressLow;
}
