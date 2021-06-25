#include "MacroInterpreter.h"
#include "Engine\Maxwell3D.h"
#include "VideoNotification.h"

CMacroInterpreter::CMacroInterpreter(CMaxwell3D & Maxwell3d) :
    m_Maxwell3d(Maxwell3d)
{
    m_MethodAddress.Value = 0;
    memset(m_Registers, 0, sizeof(m_Registers));
}

void CMacroInterpreter::AddCode(uint32_t Method, uint32_t Data)
{
    m_MacroCode[Method].push_back(Data);
}

void CMacroInterpreter::Execute(uint32_t Method, const MacroParams & Params)
{
    enum STEP_TYPE
    {
        STEP_TYPE_NORMAL,
        STEP_TYPE_DO_DELAY_SLOT,
        STEP_TYPE_JUMP,
        STEP_TYPE_EXIT_DELAY_SLOT,
        STEP_TYPE_DONE,
    };

    const CodeList & Code = m_MacroCode[Method];
    memset(m_Registers, 0, sizeof(m_Registers));
    m_MethodAddress.Value = 0;
    uint32_t PC = 0;
    STEP_TYPE NextInstruction = STEP_TYPE_NORMAL;
    bool Done = false;
    m_Registers[1] = Params[0];

    while (!Done)
    {
        if ((PC % sizeof(uint32_t)) != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (PC >= Code.size() * sizeof(uint32_t))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        MacroOpcode Opcode;
        Opcode.Value = Code[PC / sizeof(uint32_t)];

        switch (Opcode.Operation)
        {
        case MacroOperation_AddImmediate:
            ProcessResult(Params, Opcode.ResultOperation, Opcode.Dst, m_Registers[Opcode.SrcA] + Opcode.Immediate);
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        switch (NextInstruction)
        {
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (Opcode.IsExit)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }

    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void CMacroInterpreter::ProcessResult(const MacroParams & /*Params*/, MacroResultOperation Operation, uint32_t Reg,  uint32_t Result)
{
    switch (Operation)
    {
    case MacroResultOperation_MoveAndSetMethodSend:
        SetRegister(Reg, Result);
        m_MethodAddress.Value = Result;
        Send((Result >> 12) & 0x3F);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
}

void CMacroInterpreter::SetRegister(uint32_t Reg, uint32_t Value)
{
    if (Reg != 0 && Reg < (sizeof(m_Registers) / sizeof(m_Registers[0])))
    {
        m_Registers[Reg] = Value;
    }
}

void CMacroInterpreter::Send(uint32_t value)
{
    m_Maxwell3d.CallMethodFromMME((CMaxwell3D::Method)m_MethodAddress.Address, value);
    m_MethodAddress.Address += m_MethodAddress.Increment;
}
