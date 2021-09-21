#include "ShaderIR.h"
#include "ShaderOpCode.h"
#include "ShaderScanFlow.h"
#include "ShaderBranch.h"
#include "ShaderBlock.h"
#include "Renderer\Renderer.h"
#include "VideoNotification.h"
#include "Textures\Texture.h"
#include <Common\StdString.h>
#include <algorithm>

ShaderConstBuffer::ShaderConstBuffer() :
    m_MaxOffset(0),
    m_IsIndirect(false)
{
}

ShaderConstBuffer::ShaderConstBuffer(uint32_t MaxOffset, bool IsIndirect) :
    m_MaxOffset(MaxOffset),
    m_IsIndirect(IsIndirect)
{
}

void ShaderConstBuffer::MarkAsUsed(uint32_t Offset)
{
    m_MaxOffset = std::max(m_MaxOffset, Offset);
}

ShaderIR::ShaderIR(const ShaderProgramCode & ProgramCode, uint32_t MainOffset, IRenderer & Renderer) :
    m_ProgramCode(ProgramCode),
    m_MainOffset(MainOffset),
    m_Renderer(Renderer),
    m_DisableFlowStack(false)
{
    std::memcpy(&m_Header, m_ProgramCode.data(), sizeof(ShaderHeader));
    Decode();
}

ShaderIR::~ShaderIR()
{
}

void ShaderIR::Decode(void)
{
    std::unique_ptr<ShaderCharacteristics> Info = ShaderScanFlow(m_ProgramCode, m_MainOffset);
    switch (Info->Depth())
    {
    case ShaderCompileDepth_NoFlowStack:
        DecodeNoFlowStack(Info.get());
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    if (!m_Renderer.IsTextureHandlerSizeKnown() && m_UsedSamplers.size() > 1)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void ShaderIR::DecodeNoFlowStack(ShaderCharacteristics * Info)
{
    m_DisableFlowStack = true;

    uint32_t CurrentLabel = (uint32_t)ShaderBranch::ExitBranch;
    ShaderNodeList CurrentBlock;
    const ShaderBlocks & Blocks = Info->Blocks();
    for (size_t i = 0, n = Blocks.size(); i < n; i++)
    {
        const ShaderBlock & Block = Blocks[i];
        if (Info->HasLabel(Block.Start()))
        {
            if (CurrentLabel != (uint32_t)ShaderBranch::ExitBranch)
            {
                m_ShaderNodes.insert(ShaderNodeMap::value_type(CurrentLabel, CurrentBlock));
            }
            CurrentBlock.clear();
            CurrentLabel = Block.Start();
        }
        if (!Block.IgnoreBranch())
        {
            DecodeRangeInner(CurrentBlock, Block.Start(), Block.End());
            InsertControlFlow(CurrentBlock, Block);
        }
        else
        {
            DecodeRangeInner(CurrentBlock, Block.Start(), Block.End() + 1);
        }
    }
    if (CurrentLabel != (uint32_t)ShaderBranch::ExitBranch)
    {
        m_ShaderNodes.insert(ShaderNodeMap::value_type(CurrentLabel, CurrentBlock));
    }
}

void ShaderIR::DecodeRangeInner(ShaderNodeList & NodeList, uint32_t Start, uint32_t End)
{
    if (Start > End)
    {
        End = SHADER_MAX_PROGRAM_LENGTH;
    }
    for (uint32_t PC = Start; PC < End;)
    {
        if (IsShaderSchedInstruction(PC, m_MainOffset))
        {
            PC += 1;
            continue;
        }

        const ShaderInstruction & Instruction = m_ProgramCode[PC];
        ShaderOpCode Opcode(Instruction);
        uint32_t NvidiaAddress = (uint32_t)((PC - m_MainOffset) * sizeof(ShaderInstruction));
        if (!Opcode.Valid())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }

        NodeList.push_back(new ShaderCommentNode(stdstr_f("%05X %s 0x%016I64X", NvidiaAddress, Opcode.Name(), Instruction.Value).c_str()));
        if (Instruction.Pred.Full == ShaderPredIndex_NeverExecute)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }

        ShaderNodeList DecodedNodes;
        switch (Opcode.Type())
        {
        case ShaderOpCodeType_Arithmetic: PC = DecodeArithmetic(DecodedNodes, PC); break;
        case ShaderOpCodeType_Ffma: PC = DecodeFfma(DecodedNodes, PC); break;
        case ShaderOpCodeType_Conversion: PC = DecodeConversion(DecodedNodes, PC); break;
        case ShaderOpCodeType_Memory: PC = DecodeMemory(DecodedNodes, PC); break;
        case ShaderOpCodeType_Texture: PC = DecodeTexture(DecodedNodes, PC); break;
        case ShaderOpCodeType_Trivial: PC = DecodeTrivial(DecodedNodes, PC); break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
            PC = End;
            break;
        }
        bool CanBePredicated = Opcode.Id() != ShaderOpCodeId_SSY && Opcode.Id() != ShaderOpCodeId_PBK;
        if (CanBePredicated && Instruction.Pred.Index != ShaderPredIndex_Unused)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        else
        {
            for (size_t i = 0, n = DecodedNodes.size(); i < n; i++)
            {
                NodeList.push_back(DecodedNodes[i]);
            }
        }
        PC += 1;
    }
}

void ShaderIR::InsertControlFlow(ShaderNodeList & NodeList, const ShaderBlock & Block)
{
    const ShaderBranch & Branch = Block.Branch();
    if (Branch.Type() == ShaderBranchType_Single)
    {
        ShaderNodePtr Node;
        if (Branch.Address() < 0)
        {
            Node.Reset(new ShaderOperationNode(Branch.Kill() ? ShaderOperationCode_Discard : ShaderOperationCode_Exit));
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Branch.ConditionCode() != ShaderConditionCode_T)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Branch.ConditionPredicate() != ShaderPredicate_UnusedIndex)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        NodeList.push_back(Node);
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

uint32_t ShaderIR::DecodeArithmetic(ShaderNodeList & NodeList, uint32_t PC)
{
    const ShaderInstruction & Instruction = m_ProgramCode[PC];
    ShaderOpCode OpCode(Instruction);

    ShaderNodePtr OpA = GetRegister(Instruction.Gpr8);
    ShaderNodePtr OpB;
    if (Instruction.IsBImm)
    {
        OpB = GetImmediate19(Instruction);
    }
    else if (Instruction.IsBGpr)
    {
        OpB = GetRegister(Instruction.Gpr20);
    }
    else
    {
        OpB = GetConstBuffer(Instruction.CBuf34.Index, Instruction.CBuf34.GetOffset());
    }
    if (OpCode.Id() == ShaderOpCodeId_MOV_C || OpCode.Id() == ShaderOpCodeId_MOV_R)
    {
        SetRegister(NodeList, Instruction.Gpr0, OpB);
    }
    else if (OpCode.Id() == ShaderOpCodeId_FMUL_C || OpCode.Id() == ShaderOpCodeId_FMUL_R|| OpCode.Id() == ShaderOpCodeId_FMUL_IMM)
    {
        OpB = GetOperandAbsNegFloat(OpB, false, Instruction.FMul.NegateB);

        float FmulPostFactor[] = 
        {
            1.000f, // None
            0.500f, // Divide 2
            0.250f, // Divide 4
            0.125f, // Divide 8
            8.000f, // Mul 8
            4.000f, // Mul 4
            2.000f, // Mul 2
        };

        if (Instruction.FMul.PostFactor != 0)
        {
            OpA = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FMul, false, {OpA, ShaderNodePtr(new ShaderImmediateNode(FmulPostFactor[Instruction.FMul.PostFactor]))}));
        }

        ShaderNodePtr Value = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FMul, true, {OpA, OpB}));
        Value = GetSaturatedFloat(Value, Instruction.Alu.SaturateD);
        SetInternalFlagsFromFloat(NodeList, Value, Instruction.GeneratesCC);
        SetRegister(NodeList, Instruction.Gpr0, Value);
    }
    else if (OpCode.Id() == ShaderOpCodeId_FADD_C || OpCode.Id() == ShaderOpCodeId_FADD_R || OpCode.Id() == ShaderOpCodeId_FADD_IMM)
    {
        OpA = GetOperandAbsNegFloat(OpA, Instruction.Alu.AbsA, Instruction.Alu.NegateA);
        OpB = GetOperandAbsNegFloat(OpB, Instruction.Alu.AbsB, Instruction.Alu.NegateB);

        ShaderNodePtr value = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FAdd, true, {OpA, OpB}));
        value = GetSaturatedFloat(value, Instruction.Alu.SaturateD);

        SetInternalFlagsFromFloat(NodeList, value, Instruction.GeneratesCC);
        SetRegister(NodeList, Instruction.Gpr0, value);
    }
    else if (OpCode.Id() == ShaderOpCodeId_MUFU)
    {
        OpA = GetOperandAbsNegFloat(OpA, Instruction.Alu.AbsA, Instruction.Alu.NegateA);

        ShaderNodePtr Value;
        switch (Instruction.SubOP)
        {
        case ShaderSubOp_Rcp: Value = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FDiv, true, {ShaderNodePtr(new ShaderImmediateNode(1.0f)), OpA}));  break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
            Value = ShaderNodePtr(new ShaderImmediateNode(0));
        }
        Value = GetSaturatedFloat(Value, Instruction.Alu.SaturateD);
        SetRegister(NodeList, Instruction.Gpr0, Value);
    }
    else if (OpCode.Id() == ShaderOpCodeId_FMNMX_C || OpCode.Id() == ShaderOpCodeId_FMNMX_R || OpCode.Id() == ShaderOpCodeId_FMNMX_IMM)
    {
        OpA = GetOperandAbsNegFloat(OpA, Instruction.Alu.AbsA, Instruction.Alu.NegateA);
        OpB = GetOperandAbsNegFloat(OpB, Instruction.Alu.AbsB, Instruction.Alu.NegateB);

        ShaderNodePtr Condition = GetPredicate((ShaderPredicate)Instruction.Alu.FMnMx.Pred, Instruction.Alu.FMnMx.NegatePred != 0);
        ShaderNodePtr Min(new ShaderOperationNode(ShaderOperationCode_FMin, false, {OpA, OpB}));
        ShaderNodePtr Max(new ShaderOperationNode(ShaderOperationCode_FMax, false, {OpA, OpB}));
        ShaderNodePtr Value(new ShaderOperationNode(ShaderOperationCode_Select, false, {Condition, Min, Max}));
        SetInternalFlagsFromFloat(NodeList, Value, Instruction.GeneratesCC);
        SetRegister(NodeList, Instruction.Gpr0, Value);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return PC;
}

uint32_t ShaderIR::DecodeConversion(ShaderNodeList & NodeList, uint32_t PC) 
{
    const ShaderInstruction & Instruction = m_ProgramCode[PC];
    ShaderOpCode OpCode(Instruction);

    if(OpCode.Id() == ShaderOpCodeId_F2F_R || OpCode.Id() == ShaderOpCodeId_F2F_C || OpCode.Id() == ShaderOpCodeId_F2F_IMM) 
    {
        if (Instruction.Conversion.DstSize == ShaderRegisterSize_Long)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Instruction.Conversion.SrcSize == ShaderRegisterSize_Long)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Instruction.GeneratesCC)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        ShaderNodePtr Value;
        switch (OpCode.Id())
        {
        case ShaderOpCodeId_F2F_R: Value = GetRegister(Instruction.Gpr20); break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
            Value = ShaderNodePtr(new ShaderImmediateNode(0));
        }

        if (Instruction.Conversion.SrcSize == ShaderRegisterSize_Short) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        } 
        else if (Instruction.Conversion.FloatSrc.Selector != 0) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        Value = GetOperandAbsNegFloat(Value, Instruction.Conversion.AbsA, Instruction.Conversion.NegateA);
        if (Instruction.Conversion.SrcSize == Instruction.Conversion.DstSize) 
        {
            switch (Instruction.Conversion.F2F.GetRoundingMode()) 
            {
            case ShaderF2FRoundingOp_Floor: Value = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FFloor, false, {Value})); break;
            default:
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        Value = GetSaturatedFloat(Value, Instruction.Alu.SaturateD);
        SetInternalFlagsFromFloat(NodeList, Value, Instruction.GeneratesCC);
        if (Instruction.Conversion.DstSize == ShaderRegisterSize_Short) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        SetRegister(NodeList, Instruction.Gpr0, Value);
    }
    else 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    return PC;
}

uint32_t ShaderIR::DecodeFfma(ShaderNodeList & NodeList, uint32_t PC) 
{
    const ShaderInstruction & Instruction = m_ProgramCode[PC];
    ShaderOpCode OpCode(Instruction);

    if (Instruction.Ffma.CC != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    ShaderNodePtr OpA = GetRegister(Instruction.Gpr8), OpB, OpC;
    switch (OpCode.Id()) 
    {
    case ShaderOpCodeId_FFMA_IMM:
        OpB = GetImmediate19(Instruction);
        OpC = GetRegister(Instruction.Gpr39);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        OpB = ShaderNodePtr(new ShaderImmediateNode(0));
        OpC = ShaderNodePtr(new ShaderImmediateNode(0));
        break;
    }
    OpB = GetOperandAbsNegFloat(OpB, false, Instruction.Ffma.NegateB);
    OpC = GetOperandAbsNegFloat(OpC, false, Instruction.Ffma.NegateC);

    ShaderNodePtr Value = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FFma, true, {OpA, OpB, OpC}));
    Value = GetSaturatedFloat(Value, Instruction.Alu.SaturateD);
    SetInternalFlagsFromFloat(NodeList, Value, Instruction.GeneratesCC);
    SetRegister(NodeList, Instruction.Gpr0, Value);
    return PC;
}

uint32_t ShaderIR::DecodeMemory(ShaderNodeList & NodeList, uint32_t PC)
{
    const ShaderInstruction & Instruction = m_ProgramCode[PC];
    ShaderOpCode OpCode(Instruction);

    if (OpCode.Id() == ShaderOpCodeId_LD_A)
    {
        if (Instruction.Gpr8 != ShaderRegister_ZeroIndex)
        {
            g_Notify->BreakPoint(__FILE__,__LINE__);
            return PC;
        }
        if ((Instruction.Attribute.Fmt20.Immediate % sizeof(uint32_t)) != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return PC;
        }
        if (Instruction.Attribute.Fmt20.IsPhysical() && Instruction.Attribute.Fmt20.Size != ShaderAttributeSize_Word)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return PC;
        }
        ShaderNodePtr Gpr39 = GetRegister(Instruction.Gpr39);
        uint64_t NextElement = Instruction.Attribute.Fmt20.Element;
        uint64_t NextIndex = Instruction.Attribute.Fmt20.Index;
        for (uint32_t i = 0, n = Instruction.Attribute.Fmt20.Size + 1; i < n; i++)
        {
            ShaderNodePtr Attribute;
            if (Instruction.Attribute.Fmt20.IsPhysical())
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            else
            {
                Attribute.Reset(GetInputAttribute((ShaderAttributeIndex)NextIndex, NextElement, Gpr39));
            }
            SetRegister(NodeList, (uint32_t)(Instruction.Gpr0 + i), Attribute);
            NextElement = (NextElement + 1) % 4;
            NextIndex += (NextElement == 0 ? 1 : 0);
        }
    }
    else if (OpCode.Id() == ShaderOpCodeId_ST_A)
    {
        if (Instruction.Gpr8 != ShaderRegister_ZeroIndex)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return PC;
        }
        if ((Instruction.Attribute.Fmt20.Immediate % sizeof(uint32_t)) != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return PC;
        }

        uint64_t Element = Instruction.Attribute.Fmt20.Element;
        uint64_t Index = Instruction.Attribute.Fmt20.Index;
        for (uint32_t i = 0, n = Instruction.Attribute.Fmt20.Size + 1; i < n; i++)
        {
            ShaderNodePtr dest;
            if (Instruction.Attribute.Fmt20.Patch)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return PC;
            }
            ShaderNodePtr Dest = GetOutputAttribute((ShaderAttributeIndex)Index, Element, GetRegister(Instruction.Gpr39));
            ShaderNodePtr Src = GetRegister((uint32_t)(Instruction.Gpr0 + i));

            NodeList.push_back(ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_Assign, false, {Dest, Src})));
            Element = (Element + 1) % 4;
            Index += (Element == 0 ? 1 : 0);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return PC;
}

uint32_t ShaderIR::DecodeTexture(ShaderNodeList & NodeList, uint32_t PC) 
{
    const ShaderInstruction & Instruction = m_ProgramCode[PC];
    ShaderOpCode OpCode(Instruction);

    if (OpCode.Id() == ShaderOpCodeId_TEXS) 
    {
        ShaderTextureProcessMode ProcessMode = Instruction.Texs.GetTextureProcessMode();
        bool LodBiasEnabled = (ProcessMode != ShaderTextureProcessMode_None && ProcessMode != ShaderTextureProcessMode_LZ);
        bool DepthCompare = Instruction.Texs.UsesMiscMode(ShaderTextureMiscMode_DC);
        ShaderTextureType TextureType = Instruction.Texs.GetTextureType();
        bool IsArray = Instruction.Texs.IsArrayTexture();

        uint32_t CoordCount = 0;
        switch (TextureType) {
        case ShaderTextureType_1D:
            CoordCount = 1;
            break;
        case ShaderTextureType_2D:
            CoordCount = 2;
            break;
        case ShaderTextureType_3D:
            CoordCount = 3;
            break;
        case ShaderTextureType_Cube:
            CoordCount = 3;
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
            CoordCount = 0;
        }
        uint32_t TotalCoordCount = CoordCount + (IsArray ? 1 : 0) + (DepthCompare ? 1 : 0);
        uint32_t TotalRegCount = TotalCoordCount + (LodBiasEnabled ? 1 : 0);
        if (TotalCoordCount > 4 || TotalRegCount > 4) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            TotalCoordCount = std::min(TotalCoordCount, 4u);
        }
        TotalCoordCount += (DepthCompare && !IsArray && TextureType == TextureType_1D) ? 1 : 0;
        uint64_t CoordRegister = Instruction.Gpr8 + (IsArray ? 1 : 0);
        uint64_t LastCoordRegister = (IsArray || !(LodBiasEnabled || DepthCompare) || (CoordCount > 2)) ? Instruction.Gpr20 : CoordRegister + 1;
        uint32_t BiasOffset = CoordCount > 2 ? 1 : 0;

        ShaderNodeList coords;
        for (uint32_t i = 0, n = CoordCount; i < n; i++) 
        {
            bool Last = (i == (CoordCount - 1)) && (CoordCount > 1);
            coords.push_back(GetRegister((uint32_t)(Last ? LastCoordRegister : CoordRegister + i)));
        }
        ShaderNodePtr Array(IsArray ? GetRegister((uint32_t)Instruction.Gpr8) : nullptr);
        ShaderNodePtr DC(DepthCompare ? GetRegister((uint32_t)(Instruction.Gpr20 + (LodBiasEnabled ? 1 : 0))) : nullptr);
        ShaderNodeList Components = GetTextureCode(Instruction, TextureType, ProcessMode, coords, Array, DC, BiasOffset);
        if (Instruction.Texs.Fp32Flag)
        {
            WriteTexsInstructionFloat(NodeList, Instruction, Components);
        }
        else 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    } 
    else 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return PC;
}

uint32_t ShaderIR::DecodeTrivial(ShaderNodeList & NodeList, uint32_t PC)
{
    const ShaderInstruction & Instruction = m_ProgramCode[PC];
    ShaderOpCode OpCode(Instruction);

    if (OpCode.Id() == ShaderOpCodeId_IPA)
    {
        bool IsPhysical = Instruction.Ipa.Idx && Instruction.Gpr8 != 0xff;
        ShaderAttributeIndex Index = Instruction.Attribute.Fmt28.Index;
        ShaderNodePtr Value = IsPhysical ? GetPhysicalInputAttribute(Instruction.Gpr8, ShaderNodePtr(nullptr)) : GetInputAttribute(Index, Instruction.Attribute.Fmt28.Element, nullptr);
        if (Index >= ShaderAttributeIndex_Attribute_0 && Index <= ShaderAttributeIndex_Attribute_31)
        {
            uint32_t Location = Index - ShaderAttributeIndex_Attribute_0;
            if (m_Header.GetPixelImap(Location) == ShaderPixelImap_Perspective)
            {
                ShaderNodePtr PositionW = GetInputAttribute(ShaderAttributeIndex_Position, 3, nullptr);
                Value = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FMul, false, {Value, PositionW}));
            }
        }

        if (Instruction.Ipa.InterpMode == ShaderIpaInterpMode_Multiply)
        {
            Value = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FMul, false, {Value, GetRegister(Instruction.Gpr20)}));
        }
        Value = GetSaturatedFloat(Value, Instruction.Ipa.Saturate);
        SetRegister(NodeList, Instruction.Gpr0, Value);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return PC;
}

ShaderNodePtr ShaderIR::GetRegister(uint32_t Reg)
{
    if (Reg != ShaderRegister_ZeroIndex)
    {
        m_UsedRegisters.insert(Reg);
    }
    return ShaderNodePtr(new ShaderGprNode(Reg));
}

ShaderNodePtr ShaderIR::GetImmediate19(ShaderInstruction Instr)
{
    uint32_t Imm = Instr.Alu.Imm20_19;
    Imm <<= 12;
    Imm |= Instr.Alu.NegateImm ? 0x80000000 : 0;
    return ShaderNodePtr(new ShaderImmediateNode(Imm));
}

ShaderNodePtr ShaderIR::GetConstBuffer(uint32_t Index, uint64_t Offset)
{
    m_UsedConstBuffers.try_emplace(Index).first->second.MarkAsUsed((uint32_t)Offset);
    return ShaderNodePtr(new ShaderCbufNode(Index, ShaderNodePtr(new ShaderImmediateNode((uint32_t)Offset))));
}

ShaderNodePtr ShaderIR::GetPredicate(ShaderPredicate Predicate, bool Negated)
{
    if (Predicate != ShaderPredicate_UnusedIndex && Predicate != ShaderPredicate_NeverExecute)
    {
        m_UsedPredicates.insert(Predicate);
    }
    return ShaderNodePtr(new ShaderPredicateNode(Predicate, Negated));
}

ShaderNodePtr ShaderIR::GetInputAttribute(ShaderAttributeIndex Index, uint64_t Element, ShaderNodePtr Buffer)
{
    MarkAttributeUsage(Index, Element);
    m_UsedInputAttributes.emplace(Index);
    return ShaderNodePtr(new ShaderAttributeBufferNode(Index, (uint32_t)Element,Buffer));
}

ShaderNodePtr ShaderIR::GetPhysicalInputAttribute(uint64_t /*PhysicalAddress*/, ShaderNodePtr /*Buffer*/)
{
    g_Notify->BreakPoint(__FILE__,__LINE__);
    return ShaderNodePtr(nullptr);
}

ShaderNodePtr ShaderIR::GetOutputAttribute(ShaderAttributeIndex Index, uint64_t Element, ShaderNodePtr Buffer)
{
    MarkAttributeUsage(Index, Element);
    m_UsedOutputAttributes.insert(Index);
    return ShaderNodePtr(new ShaderAttributeBufferNode(Index, (uint32_t)Element, Buffer));
}

ShaderNodePtr ShaderIR::GetInternalFlag(ShaderInternalFlag /*Flag*/, bool /*Negated*/) const
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return ShaderNodePtr(nullptr);
}

ShaderNodePtr ShaderIR::GetOperandAbsNegFloat(ShaderNodePtr Value, bool Absolute, bool Negate)
{
    if (Absolute) 
    {
        Value = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FAbsolute, false, {Value}));
    }
    if (Negate)
    {
        Value = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_FNegate, false, {Value}));
    }
    return Value;
}

ShaderNodePtr ShaderIR::GetSaturatedFloat(ShaderNodePtr Value, bool Saturate)
{
    if (!Saturate)
    {
        return Value;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return Value;
}

void ShaderIR::SetRegister(ShaderNodeList & NodeList, uint32_t dest, ShaderNodePtr src)
{
    NodeList.push_back(ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_Assign, false, { GetRegister(dest), std::move(src) })));
}

void ShaderIR::SetInternalFlag(ShaderNodeList & NodeList, ShaderInternalFlag Flag, ShaderNodePtr Value)
{
    NodeList.push_back(ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_LogicalAssign, false, { GetInternalFlag(Flag, false), Value })));
}

void ShaderIR::SetInternalFlagsFromFloat(ShaderNodeList & NodeList, ShaderNodePtr Value, bool SetsCC)
{
    if (!SetsCC)
    {
        return;
    }
    ShaderNodePtr ZeroNode = ShaderNodePtr(new ShaderOperationNode(ShaderOperationCode_LogicalFOrdEqual, false, { Value, ShaderNodePtr(new ShaderImmediateNode(0.0f)) }));
    SetInternalFlag(NodeList, ShaderInternalFlag_Zero, ZeroNode);
}

void ShaderIR::MarkAttributeUsage(ShaderAttributeIndex Index, uint64_t /*Element*/)
{
    if (Index >= ShaderAttributeIndex_Attribute_0 && Index <= ShaderAttributeIndex_Attribute_31)
    {
        return;
    }

    switch (Index)
    {
    case ShaderAttributeIndex_Position:
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
}

ShaderNodeList ShaderIR::GetTextureCode(ShaderInstruction Instruction, ShaderTextureType TextureType, ShaderTextureProcessMode ProcessMode, ShaderNodeList Coords, ShaderNodePtr Array, ShaderNodePtr DepthCompare, uint32_t /*BiasOffset*/)
{
    bool LodNeeded = ProcessMode == ShaderTextureProcessMode_LZ || ProcessMode == ShaderTextureProcessMode_LL || ProcessMode == ShaderTextureProcessMode_LLA;
    ShaderOperationCode OpCode = LodNeeded ? ShaderOperationCode_TextureLod : ShaderOperationCode_Texture;
    bool IsArray = Array.get() != nullptr;
    bool IsShadow = DepthCompare.get() != nullptr;

    if (TextureType == ShaderTextureType_3D && IsArray && IsShadow)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    const ShaderSamplerEntry * Sampler = nullptr;
    for (ShaderSamplerEntryList::const_iterator itr = m_UsedSamplers.begin(); itr != m_UsedSamplers.end(); itr++) 
    {
        if (itr->Offset() != Instruction.Sampler.Index)
        {
            continue;
        }
        if (itr->IsBindless() || itr->Type() != TextureType || itr->IsArray() != IsArray ||
            itr->IsShadow() != IsShadow || itr->IsBuffer() != false) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        Sampler = &(*itr);
        break;
    }

    if (Sampler == nullptr) 
    {
        m_UsedSamplers.emplace_back((uint32_t)m_UsedSamplers.size(), (uint32_t)Instruction.Sampler.Index, TextureType, IsArray, IsShadow, false, false);
        Sampler = &m_UsedSamplers[m_UsedSamplers.size() - 1];
    }

    ShaderNodePtr IndexVar, Bias, Lod;
    switch (ProcessMode)
    {
    case ShaderTextureProcessMode_None:
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }

    ShaderNodeList Values;
    Values.resize(4);
    for (uint32_t i = 0; i < 4; i++) 
    {
        Values[i] = ShaderNodePtr(new ShaderOperationNode(OpCode, ShaderTexture(*Sampler, Array, DepthCompare, {}, {}, {}, Bias,  Lod, {}, i, IndexVar), Coords));
    }
    return Values;
}

void ShaderIR::WriteTexsInstructionFloat(ShaderNodeList & NodeList, ShaderInstruction Instruction, const ShaderNodeList & Components) 
{
    uint32_t DestElement = 0;
    for (uint32_t i = 0, n = (uint32_t)Components.size(); i < n; i++)
    {
        if (!Instruction.Texs.IsComponentEnabled(i)) 
        {
            continue;
        }
        SetRegister(NodeList, ShaderRegister_ZeroIndex + 1 + DestElement, Components[i]);
        DestElement += 1;
    }

    for (uint32_t i = 0; i < DestElement; i++) 
    {
        if (i >= 2 && !Instruction.Texs.HasTwoDestinations()) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }
        SetRegister(NodeList, (uint32_t)((i < 2 ? Instruction.Gpr0 : Instruction.Gpr28) + i % 2), GetRegister(ShaderRegister_ZeroIndex + 1 + i));
    }
}
