#pragma once
#include "Shader.h"
#include "ShaderHeader.h"
#include "ShaderNode.h"
#include "Engine/Maxwell3D.h"
#include <stdint.h>
#include <map>
#include <set>

class CVideo;
class ShaderCharacteristics;
class ShaderBlock;
__interface IRenderer;

typedef struct
{
    CMaxwell3D::tyTransformFeedbackLayout TFBLayouts[CMaxwell3D::NumTransformFeedbackBuffers];
    uint8_t TFBVaryingLocs[CMaxwell3D::NumTransformFeedbackBuffers][128];
    CMaxwell3D::PrimitiveTopology PrimitiveTopology;
    CMaxwell3D::TessellationPrimitive TessellationPrimitive;
    CMaxwell3D::TessellationSpacing TessellationSpacing;
    bool TFBEnabled;
    bool TessellationClockwise;
} ShaderGraphicsInfo;

class ShaderConstBuffer 
{
public:
    ShaderConstBuffer();
    ShaderConstBuffer(uint32_t MaxOffset, bool IsIndirect);

    void MarkAsUsed(uint32_t Offset);

    bool IsIndirect() const { return m_IsIndirect; }
    uint32_t GetSize() const { return m_MaxOffset + (uint32_t)(sizeof(float)); }
    uint32_t GetMaxOffset() const { return m_MaxOffset; }

private:
    ShaderConstBuffer& operator=(const ShaderConstBuffer&);

    uint32_t m_MaxOffset;
    bool m_IsIndirect;
};

typedef std::map<uint32_t, ShaderConstBuffer> ShaderConstBufferMap;

class ShaderIR 
{
public:
    typedef std::map<uint32_t, ShaderNodeList> ShaderNodeMap;
    typedef std::set<uint32_t> RegisterSet;
    typedef std::set<ShaderPredicate> PredicateSet;

    ShaderIR(const ShaderProgramCode & ProgramCode, uint32_t MainOffset, IRenderer & Renderer, CVideo & Video, ShaderType Type);
    ~ShaderIR();

    const ShaderNodeMap & ShaderNodes() const { return m_ShaderNodes; }
    const RegisterSet & GetRegisters() const { return m_UsedRegisters; }
    const PredicateSet & GetPredicates() const { return m_UsedPredicates; }
    const ShaderAttributeIndexSet & GetInputAttributes() const { return m_UsedInputAttributes; }
    const ShaderAttributeIndexSet & GetOutputAttributes() const { return m_UsedOutputAttributes; }
    const ShaderConstBufferMap & GetConstantBuffers() const { return m_UsedConstBuffers; }
    const ShaderSamplerEntryList & GetSamplers() const { return m_UsedSamplers; }
    bool HasPhysicalAttributes() const { return m_UsesPhysicalAttributes; }
    bool IsDecompiled() const { return m_Decompiled; }
    bool IsFlowStackDisabled() const { return m_DisableFlowStack; }
    const ShaderHeader & GetHeader() const { return m_Header; }
    const ShaderGraphicsInfo & GetGraphicsInfo() const;

private:
    ShaderIR();
    ShaderIR(const ShaderIR&);
    ShaderIR& operator=(const ShaderIR&);

    void Decode();
    void DecodeNoFlowStack(ShaderCharacteristics * Info);
    void DecodeRangeInner(ShaderNodeList & NodeList, uint32_t Start, uint32_t End);
    void InsertControlFlow(ShaderNodeList & NodeList, const ShaderBlock & Block);
    
    uint32_t DecodeArithmetic(ShaderNodeList & NodeList, uint32_t PC);
    uint32_t DecodeConversion(ShaderNodeList & NodeList, uint32_t PC);
    uint32_t DecodeFfma(ShaderNodeList & NodeList, uint32_t PC);
    uint32_t DecodeMemory(ShaderNodeList & NodeList, uint32_t PC);
    uint32_t DecodeTexture(ShaderNodeList & NodeList, uint32_t PC);
    uint32_t DecodeTrivial(ShaderNodeList & NodeList, uint32_t PC);

    ShaderNodePtr GetRegister(uint32_t Register);
    ShaderNodePtr GetImmediate19(ShaderInstruction Instr);
    ShaderNodePtr GetConstBuffer(uint32_t Index, uint64_t Offset);
    ShaderNodePtr GetPredicate(ShaderPredicate Predicate, bool Negated);
    ShaderNodePtr GetInputAttribute(ShaderAttributeIndex Index, uint64_t Element, ShaderNodePtr Buffer);
    ShaderNodePtr GetPhysicalInputAttribute(uint64_t PhysicalAddress, ShaderNodePtr Buffer);
    ShaderNodePtr GetOutputAttribute(ShaderAttributeIndex Index, uint64_t Element, ShaderNodePtr Buffer);
    ShaderNodePtr GetInternalFlag(ShaderInternalFlag Flag, bool Negated) const;
    void SetRegister(ShaderNodeList & NodeList, uint32_t Dest, ShaderNodePtr Src);
    void SetInternalFlag(ShaderNodeList & NodeList, ShaderInternalFlag Flag, ShaderNodePtr Value);
    void SetInternalFlagsFromFloat(ShaderNodeList & NodeList, ShaderNodePtr Value, bool SetsCC);
    ShaderNodePtr GetOperandAbsNegFloat(ShaderNodePtr Value, bool Absolute, bool Negate);
    ShaderNodePtr GetSaturatedFloat(ShaderNodePtr Value, bool Saturate);
    void MarkAttributeUsage(ShaderAttributeIndex Index, uint64_t Element);
    ShaderNodeList GetTextureCode(ShaderInstruction Instruction, ShaderTextureType TextureType, ShaderTextureProcessMode ProcessMode, ShaderNodeList Coords, ShaderNodePtr Array, ShaderNodePtr DepthCompare, uint32_t BiasOffset);
    void WriteTexsInstructionFloat(ShaderNodeList & NodeList, ShaderInstruction Instruction, const ShaderNodeList & components);

    static ShaderGraphicsInfo MakeGraphicsInfo(ShaderType Type, CMaxwell3D & Maxwell3D);

    const ShaderProgramCode & m_ProgramCode;
    uint32_t m_MainOffset;
    IRenderer & m_Renderer;
    CVideo & m_Video;
    ShaderType m_ShaderType;
    ShaderHeader m_Header;
    ShaderGraphicsInfo m_GraphicsInfo;
    bool m_Decompiled;
    bool m_DisableFlowStack;
    ShaderNodeMap m_ShaderNodes;
    RegisterSet m_UsedRegisters;
    PredicateSet m_UsedPredicates;
    ShaderAttributeIndexSet m_UsedInputAttributes;
    ShaderAttributeIndexSet m_UsedOutputAttributes;
    ShaderConstBufferMap m_UsedConstBuffers;
    ShaderSamplerEntryList m_UsedSamplers;
    bool m_UsesPhysicalAttributes;
};
