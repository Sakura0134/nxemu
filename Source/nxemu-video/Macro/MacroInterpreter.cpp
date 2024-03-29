#include "MacroInterpreter.h"
#include "Engine\Maxwell3D.h"
#include "VideoNotification.h"

CMacroInterpreter::CMacroInterpreter(CMaxwell3D & Maxwell3d) :
    m_Maxwell3d(Maxwell3d),
    m_NextParam(0),
    m_Carry(false)
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
    m_NextParam = 1;
    m_Carry = false;
    uint32_t PC = 0, JumpPC = 0;
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
        case MacroOperation_ALU:
            if (Opcode.SrcA < (sizeof(m_Registers) / sizeof(m_Registers[0])) && Opcode.SrcB < (sizeof(m_Registers) / sizeof(m_Registers[0])))
            {
                uint32_t Result = GetALUResult(Opcode.AluOperation, m_Registers[Opcode.SrcA], m_Registers[Opcode.SrcB]);
                ProcessResult(Params, Opcode.ResultOperation, Opcode.Dst, Result);
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            break;
        case MacroOperation_AddImmediate:
            ProcessResult(Params, Opcode.ResultOperation, Opcode.Dst, m_Registers[Opcode.SrcA] + Opcode.Immediate);
            break;
        case MacroOperation_ExtractInsert:
            if (Opcode.SrcA < (sizeof(m_Registers) / sizeof(m_Registers[0])) && Opcode.SrcB < (sizeof(m_Registers) / sizeof(m_Registers[0])))
            {
                uint32_t Src = (m_Registers[Opcode.SrcB] >> Opcode.BfSrcBit) & Opcode.GetBitfieldMask();
                uint32_t Dst = m_Registers[Opcode.SrcA] & (~(Opcode.GetBitfieldMask() << Opcode.BfDstBit));
                Dst |= Src << Opcode.BfDstBit;
                ProcessResult(Params, Opcode.ResultOperation, Opcode.Dst, Dst);
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            break;
        case MacroOperation_Read:
            if ((m_Registers[Opcode.SrcA] + Opcode.Immediate) < CMaxwell3D::NumRegisters)
            {
                uint32_t Result = m_Maxwell3d.Regs().Value[m_Registers[Opcode.SrcA] + Opcode.Immediate];
                ProcessResult(Params, Opcode.ResultOperation, Opcode.Dst, Result);
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            break;
        case MacroOperation_Branch:
            if (Opcode.SrcA < (sizeof(m_Registers) / sizeof(m_Registers[0])) && NextInstruction == STEP_TYPE_NORMAL)
            {
                uint32_t value = m_Registers[Opcode.SrcA];
                bool Branch = (Opcode.BranchCondition == 0 && value == 0) || (Opcode.BranchCondition == 1 && value != 0);
                if (Branch)
                {
                    JumpPC = PC + (Opcode.Immediate << 2);
                    NextInstruction = Opcode.BranchAnnul ? STEP_TYPE_JUMP : STEP_TYPE_DO_DELAY_SLOT;
                }
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        switch (NextInstruction)
        {
        case STEP_TYPE_NORMAL:
            PC += 4;
            break;
        case STEP_TYPE_DO_DELAY_SLOT:
            NextInstruction = STEP_TYPE_JUMP;
            PC += 4;
            break;
        case STEP_TYPE_JUMP:
            NextInstruction = STEP_TYPE_NORMAL;
            PC = JumpPC;
            break;
        case STEP_TYPE_EXIT_DELAY_SLOT:
            NextInstruction = STEP_TYPE_DONE;
            Done = true;
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        if (Opcode.IsExit)
        {
            if (NextInstruction == STEP_TYPE_JUMP)
            {
                continue;
            }
            if (NextInstruction != STEP_TYPE_NORMAL)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return;
            }
            NextInstruction = STEP_TYPE_EXIT_DELAY_SLOT;
        }
    }

    if (m_NextParam != Params.size())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

uint32_t CMacroInterpreter::GetALUResult(MacroALUOperation Operation, uint32_t SrcA, uint32_t SrcB)
{
    switch (Operation)
    {
    case MacroALUOperation_Subtract:
        {
            uint64_t Result = ((uint64_t)SrcA) - SrcB;
            m_Carry = Result < 0x100000000;
            return (uint32_t)Result;
        }
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

void CMacroInterpreter::ProcessResult(const MacroParams & Params, MacroResultOperation Operation, uint32_t Reg,  uint32_t Result)
{
    switch (Operation)
    {
    case MacroResultOperation_IgnoreAndFetch:
        if (m_NextParam >= Params.size())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        SetRegister(Reg, Params[m_NextParam++]);
        break;
    case MacroResultOperation_Move:
        SetRegister(Reg, Result);
        break;
    case MacroResultOperation_MoveAndSetMethod:
        SetRegister(Reg, Result);
        m_MethodAddress.Value = Result;
        break;
    case MacroResultOperation_FetchAndSend:
        if (m_NextParam >= Params.size())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        SetRegister(Reg, Params[m_NextParam++]);
        Send(Result);
        break;
    case MacroResultOperation_MoveAndSend:
        SetRegister(Reg, Result);
        Send(Result);
        break;
    case MacroResultOperation_FetchAndSetMethod:
        if (m_NextParam >= Params.size())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        SetRegister(Reg, Params[m_NextParam++]);
        m_MethodAddress.Value = Result;
        break;
    case MacroResultOperation_MoveAndSetMethodFetchAndSend:
        if (m_NextParam >= Params.size())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        SetRegister(Reg, Result);
        m_MethodAddress.Value = Result;
        Send(Params[m_NextParam++]);
        break;
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
