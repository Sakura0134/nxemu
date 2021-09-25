#include "OpenGLShaderDecompiler.h"
#include "OpenGLShaderWriter.h"
#include "OpenGLExpression.h"
#include "OpenGLDevice.h"
#include "VideoNotification.h"
#include <Common/Align.h>
#include <Common/StdString.h>
#include <unordered_map>
#include <algorithm>

static const char * FloatType[] = 
{
    "float", 
    "vec2", 
    "vec3", 
    "vec4"
};

class OpenGLShaderDecompiler 
{
    enum
    {
        MAX_CONSTBUFFER_SCALARS = (uint32_t)(CMaxwell3D::MaxConstBufferSize / sizeof(uint32_t))
    };

    struct VaryingTFB 
    {
        uint64_t Buffer;
        uint64_t Stride;
        uint64_t Offset;
        uint64_t Components;
    };

    struct VaryingDescription 
    {
        std::string Name;
        uint8_t FirstElement;
        bool IsScalar;
    };

    typedef std::unordered_map<uint8_t, VaryingTFB> VaryingTFBMap;
    typedef std::unordered_map<uint8_t, VaryingDescription> VaryingDescriptionMap;

public:
    OpenGLShaderDecompiler(const OpenGLDevice & Device, const ShaderIR & ShaderIR, ShaderType Stage, const char * Identifier);

    void Decompile();
    std::string GetResult();

    static void InitOperationDecompilers(void);

private:
    typedef OpenGLExpression (OpenGLShaderDecompiler::*fnOperationDecompiler)(const ShaderOperationNode & Operation);
    static fnOperationDecompiler m_OperationDecompilers[ShaderOperationCode_Amount];

    VaryingTFBMap BuildTransformFeedback(const ShaderGraphicsInfo & info);
    void DecompileBranchMode(); 
    void DeclareCompute();
    void DeclareConstantBuffers();
    void DeclareFragment();
    void DeclareGeometry();
    void DeclareHeader();
    void DeclareInputAttribute(ShaderAttributeIndex Index);
    void DeclareInputAttributes();
    void DeclareInternalFlags();
    void DeclareLocalMemory();
    void DeclareOutputAttribute(ShaderAttributeIndex Index);
    void DeclareOutputAttributes();
    void DeclarePhysicalAttributeReader();
    void DeclarePredicates();
    void DeclareRegisters();
    void DeclareSamplers();
    void DeclareVertex();
    std::string GetTransformFeedbackDecoration(ShaderAttributeIndex Index, uint8_t Element = 0) const;
    std::string GetTextureCode(const ShaderOperationNode & Operation, OpenGLExpressionType ArgumentType, ShaderNodePtr ArgumentNode, bool HasOffset, bool SeparateDC);
    OpenGLExpression ApplyPrecise(const ShaderOperationNode & Operation, const char * Value, OpenGLExpressionType Type);
    OpenGLExpression GetOutputAttribute(const ShaderAttributeBufferNode* AttributeBuffer);
    OpenGLExpression ReadAttribute(ShaderAttributeIndex Attribute, uint32_t Element, const ShaderNodePtr & Buffer); 
    OpenGLExpression Visit(const ShaderNodePtr & Node);
    OpenGLExpression VisitOperand(const ShaderOperationNode & Operation, uint32_t OperandIndex);

    OpenGLExpression Assign(const ShaderOperationNode & Operation);
    OpenGLExpression Exit(const ShaderOperationNode & Operation);
    OpenGLExpression FAdd(const ShaderOperationNode & Operation);
    OpenGLExpression FDiv(const ShaderOperationNode & Operation);
    OpenGLExpression FFloor(const ShaderOperationNode & Operation);
    OpenGLExpression FFma(const ShaderOperationNode & Operation);
    OpenGLExpression FMax(const ShaderOperationNode & Operation);
    OpenGLExpression FMin(const ShaderOperationNode & Operation);
    OpenGLExpression FMul(const ShaderOperationNode & Operation);
    OpenGLExpression FNegate(const ShaderOperationNode & Operation);
    OpenGLExpression Select(const ShaderOperationNode & Operation);
    OpenGLExpression Texture(const ShaderOperationNode & Operation);

    static bool UseUnifiedUniforms(const OpenGLDevice & Device, const ShaderIR & ShaderIR, ShaderType Stage);
    static bool IsGenericAttribute(ShaderAttributeIndex Index);
    static const char * GetInputFlags(ShaderPixelImap Attribute);
    static uint32_t GetGenericAttributeIndex(ShaderAttributeIndex Index); 
    static std::string GetGenericInputAttribute(ShaderAttributeIndex Attribute);
    static const char * GetInternalFlag(ShaderInternalFlag Flag);
    static const char * GetTypeString(OpenGLExpressionType Type);
    static const char * GetSwizzle(uint32_t Element); 
    static bool IsPrecise(const ShaderNodePtr & Node);

    const OpenGLDevice & m_Device;
    const ShaderIR & m_ShaderIR;
    const ShaderType m_Stage;
    const char * m_Identifier;
    const ShaderHeader m_Header;
    bool m_UseUnifiedUniforms;
    VaryingTFBMap m_TransformFeedback;
    VaryingDescriptionMap m_VaryingDescription;
    OpenGLShaderWriter m_Code;
};

OpenGLShaderConstBuffer::OpenGLShaderConstBuffer(uint32_t MaxOffset, bool IsIndirect, uint32_t Index) :
    ShaderConstBuffer(MaxOffset, IsIndirect),
    m_Index(Index)
{
}

OpenGLShaderDecompiler::fnOperationDecompiler OpenGLShaderDecompiler::m_OperationDecompilers[ShaderOperationCode_Amount];
void InitOpenGLDecompileShader(void)
{
    OpenGLShaderDecompiler::InitOperationDecompilers();
}

std::string OpenGLDecompileShader(const OpenGLDevice & Device, const ShaderIR & IR, ShaderType Stage, const char * Identifier)
{
    OpenGLShaderDecompiler Decompiler(Device, IR, Stage, Identifier);
    Decompiler.Decompile();
    return Decompiler.GetResult();
}

OpenGLShaderDecompiler::OpenGLShaderDecompiler(const OpenGLDevice & Device, const ShaderIR & ShaderIR, ShaderType Stage, const char * Identifier) : 
    m_Device(Device), 
    m_ShaderIR(ShaderIR), 
    m_Stage(Stage), 
    m_Identifier(Identifier),
    m_Header(m_ShaderIR.GetHeader()),
    m_UseUnifiedUniforms(UseUnifiedUniforms(Device, ShaderIR, Stage)) 
{
    if (m_Stage != ShaderType_Compute) 
    {
        m_TransformFeedback = BuildTransformFeedback(m_ShaderIR.GetGraphicsInfo());
    }
}

std::string OpenGLShaderDecompiler::GetTransformFeedbackDecoration(ShaderAttributeIndex Index, uint8_t Element) const 
{
    uint8_t Location = (uint8_t)((Index * 4) + Element);
    VaryingTFBMap::const_iterator itr = m_TransformFeedback.find(Location);
    if (itr == m_TransformFeedback.end()) 
    {
        return "";
    }
    const VaryingTFB & TFB = itr->second;
    return stdstr_f("xfb_buffer = %I64d, xfb_offset = %I64d, xfb_stride = %I64d", (uint64_t)TFB.Buffer, (uint64_t)TFB.Offset, (uint64_t)TFB.Stride);
}

std::string OpenGLShaderDecompiler::GetTextureCode(const ShaderOperationNode & Operation, OpenGLExpressionType ArgumentType, ShaderNodePtr ArgumentNode, bool HasOffset, bool SeparateDC)
{
    if (Operation.OperationType() != ShaderOperationType_Texture)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return "";
    }
    const ShaderTexture & Texture = Operation.GetTexture();
    const ShaderSamplerEntry & Sampler = Texture.Sampler();

    size_t count = Operation.GetOperandsCount();
    bool HasArray = Sampler.IsArray();
    bool HasShadow = Sampler.IsShadow();

    std::string TextureCode = "texture";
    if (!Texture.AOffi().empty())
    {
        TextureCode += "Offset";
    } 
    else if (!Texture.PTP().empty()) 
    {
        TextureCode += "Offsets";
    }
    TextureCode += stdstr_f("(sampler%d",Sampler.Index());
    if (Sampler.IsIndexed())
    {
        TextureCode += '[' + Visit(Texture.Index()).AsUint() + "]";
    }
    TextureCode += ", ";
    const char * Coords[] = { "float", "vec2", "vec3", "vec4" };
    TextureCode += Coords[count + (HasArray ? 1 : 0) + (HasShadow && !SeparateDC ? 1 : 0) - 1];
    TextureCode += '(';
    for (size_t i = 0; i < count; i++) 
    {
        TextureCode += Visit(Operation.GetOperand(i)).AsFloat();
        if (i + 1 < count) 
        {
            TextureCode += ", ";
        }
    }
    if (HasArray) 
    {
        TextureCode += ", float(" + Visit(Texture.Array()).AsInt() + ')';
    }
    if (HasShadow) 
    {
        if (SeparateDC) { TextureCode += ")"; }
        TextureCode += ", " + Visit(Texture.DepthCompare()).AsFloat();
        if (!SeparateDC) { TextureCode += ")"; }
    } 
    else 
    {
        TextureCode += ')';
    }

    if (HasOffset) 
    {
        if (!Texture.AOffi().empty()) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        } 
        else if (!Texture.PTP().empty())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }    
    }
    if (ArgumentNode.get() != nullptr) 
    {
        TextureCode += ", ";
        switch (ArgumentType) 
        {
        case OpenGLExpressionType_Int:
            if (ArgumentNode->GetShaderNodeType() == ShaderNodeType_Immediate) 
            {
                TextureCode += stdstr_f("%d", ArgumentNode->GetShaderImmediateNode()->GetValue());
            }
            else
            {
                TextureCode += Visit(ArgumentNode).AsInt();
            }
            break;
        case OpenGLExpressionType_Float:
            TextureCode += Visit(ArgumentNode).AsFloat();
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
            TextureCode += '0';
            break;
        }
    }
    return TextureCode + ')';
}

void OpenGLShaderDecompiler::Decompile() 
{
    DeclareHeader();
    DeclareVertex();
    DeclareGeometry();
    DeclareFragment();
    DeclareCompute();
    DeclareInputAttributes();
    DeclareOutputAttributes();
    DeclareSamplers();
    DeclareConstantBuffers();
    DeclareLocalMemory();
    DeclareRegisters();
    DeclarePredicates();
    DeclareInternalFlags();
    DeclarePhysicalAttributeReader();

    m_Code.AddCode("void main() {");
    m_Code.ScopeIncrease();

    if (m_Stage == ShaderType_Vertex)
    {
        m_Code.AddCode("gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);");
    }

    if (m_ShaderIR.IsDecompiled()) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    } 
    else 
    {
        DecompileBranchMode();
    }

    m_Code.ScopeDecrease();
    m_Code.AddCode("}");
}

std::string OpenGLShaderDecompiler::GetResult()
{
    return m_Code.Code();
}

void OpenGLShaderDecompiler::InitOperationDecompilers(void) 
{
    memset(m_OperationDecompilers, 0, sizeof(m_OperationDecompilers));
    m_OperationDecompilers[ShaderOperationCode_Assign] = &OpenGLShaderDecompiler::Assign;
    m_OperationDecompilers[ShaderOperationCode_Exit] = &OpenGLShaderDecompiler::Exit;
    m_OperationDecompilers[ShaderOperationCode_FAdd] = &OpenGLShaderDecompiler::FAdd;
    m_OperationDecompilers[ShaderOperationCode_FDiv] = &OpenGLShaderDecompiler::FDiv;
    m_OperationDecompilers[ShaderOperationCode_FFloor] = &OpenGLShaderDecompiler::FFloor;
    m_OperationDecompilers[ShaderOperationCode_FFma] = &OpenGLShaderDecompiler::FFma;
    m_OperationDecompilers[ShaderOperationCode_FMax] = &OpenGLShaderDecompiler::FMax;
    m_OperationDecompilers[ShaderOperationCode_FMin] = &OpenGLShaderDecompiler::FMin;
    m_OperationDecompilers[ShaderOperationCode_FMul] = &OpenGLShaderDecompiler::FMul;
    m_OperationDecompilers[ShaderOperationCode_FNegate] = &OpenGLShaderDecompiler::FNegate;
    m_OperationDecompilers[ShaderOperationCode_Select] = &OpenGLShaderDecompiler::Select;
    m_OperationDecompilers[ShaderOperationCode_Texture] = &OpenGLShaderDecompiler::Texture;
}

void OpenGLShaderDecompiler::DecompileBranchMode() 
{
    const ShaderIR::ShaderNodeMap & ShaderNodes = m_ShaderIR.ShaderNodes();
    m_Code.AddCode("uint jmp_to = %dU;", ShaderNodes.begin()->first);

    if (!m_ShaderIR.IsFlowStackDisabled()) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    m_Code.AddCode("while (true) {");
    m_Code.ScopeIncrease();
    m_Code.AddCode("switch (jmp_to) {");
    for (ShaderIR::ShaderNodeMap::const_iterator itr = ShaderNodes.begin(); itr != ShaderNodes.end(); itr++)
    {
        m_Code.AddCode("case 0x%XU: {", itr->first);
        m_Code.ScopeIncrease();

        const ShaderNodeList & Nodes = itr->second;
        for (uint32_t i = 0, n = (uint32_t)Nodes.size(); i < n; i++) 
        {
            Visit(Nodes[i]).CheckVoid();
        }

        m_Code.ScopeDecrease();
        m_Code.AddCode("}");
    }
    m_Code.AddCode("default: return;");
    m_Code.AddCode("}");

    m_Code.ScopeDecrease();
    m_Code.AddCode("}");
}

void OpenGLShaderDecompiler::DeclareCompute()
{
    if (m_Stage != ShaderType_Compute)
    {
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void OpenGLShaderDecompiler::DeclareConstantBuffers() 
{
    enum 
    {
        MAX_CONSTBUFFER_ELEMENTS = MAX_CONSTBUFFER_SCALARS / sizeof(uint32_t)
    };

    if (m_UseUnifiedUniforms)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    uint32_t Binding = m_Device.GetBaseBindings(m_Stage).UniformBuffer;
    const ShaderConstBufferMap & ConstBuffers = m_ShaderIR.GetConstantBuffers();
    for (ShaderConstBufferMap::const_iterator itr = ConstBuffers.begin(); itr != ConstBuffers.end(); itr++) 
    {
        const ShaderConstBuffer & ConstBuffer = itr->second;
        uint32_t Elements = AlignUp(ConstBuffer.GetSize(), 4) / 4;
        uint32_t Size = ConstBuffer.IsIndirect() ? MAX_CONSTBUFFER_ELEMENTS : Elements;
        m_Code.AddCode("layout (std140, binding = %d) uniform cbuf_block%d {", Binding++, itr->first);
        m_Code.AddCode("    uvec4 cbuf%d[%d];", itr->first, Size);
        m_Code.AddCode("};");
        m_Code.AddCode("");
    }
}

void OpenGLShaderDecompiler::DeclareFragment()
{
    if (m_Stage != ShaderType_Fragment)
    {
        return;
    }
    for (uint32_t i = 0; i < NumRenderTargets; i++)
    {
        m_Code.AddCode("layout (location = %d) out vec4 frag_color%d;", i, i);
    }
}

void OpenGLShaderDecompiler::DeclareGeometry() 
{
    if (m_Stage != ShaderType_Geometry) 
    {
        return;
    }

    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void OpenGLShaderDecompiler::DeclareHeader() 
{
    const char* CommonDeclarations = "#define ftoi floatBitsToInt\n\
#define ftou floatBitsToUint\n\
#define itof intBitsToFloat\n\
#define utof uintBitsToFloat\n\
\n\
bvec2 HalfFloatNanComparison(bvec2 comparison, vec2 pair1, vec2 pair2) {\n\
    bvec2 is_nan1 = isnan(pair1);\n\
    bvec2 is_nan2 = isnan(pair2);\n\
    return bvec2(comparison.x || is_nan1.x || is_nan2.x, comparison.y || is_nan1.y || is_nan2.y);\n\
}\n\
\n\
const float fswzadd_modifiers_a[] = float[4](-1.0f,  1.0f, -1.0f,  0.0f );\n\
const float fswzadd_modifiers_b[] = float[4](-1.0f, -1.0f,  1.0f, -1.0f );\n\
\n\
layout (std140, binding = 0) uniform vs_config {\n\
    float y_direction;\n\
};\n";

    if (m_Identifier != nullptr)
    {
        m_Code.AddCode("// %s", m_Identifier);
    }
    m_Code.AddCode("#version 440 core");
    m_Code.AddCode("#extension GL_ARB_separate_shader_objects : enable");
    if (m_Device.HasShaderBallot())
    {
        m_Code.AddCode("#extension GL_ARB_shader_ballot : require");
    }
    if (m_Device.HasVertexViewportLayer()) 
    {
        m_Code.AddCode("#extension GL_ARB_shader_viewport_layer_array : require");
    }
    if (m_Device.HasImageLoadFormatted()) 
    {
        m_Code.AddCode("#extension GL_EXT_shader_image_load_formatted : require");
    }
    if (m_Device.HasTextureShadowLod())
    {
        m_Code.AddCode("#extension GL_EXT_texture_shadow_lod : require");
    }
    if (m_Device.HasWarpIntrinsics()) 
    {
        m_Code.AddCode("#extension GL_NV_gpu_shader5 : require");
        m_Code.AddCode("#extension GL_NV_shader_thread_group : require");
        m_Code.AddCode("#extension GL_NV_shader_thread_shuffle : require");
    }
    m_Code.AddCode("#pragma optionNV(fastmath off)");
    m_Code.AddCode("");
    m_Code.AddCode(CommonDeclarations);
}

void OpenGLShaderDecompiler::DeclareInputAttributes() 
{
    const ShaderAttributeIndexSet & Attributes = m_ShaderIR.GetInputAttributes();
    for (ShaderAttributeIndexSet::const_iterator itr = Attributes.begin(); itr != Attributes.end(); itr++)
    {
        ShaderAttributeIndex Attribute = *itr;
        if (IsGenericAttribute(Attribute))
        {
            DeclareInputAttribute(Attribute);
        }
    }
    if (!Attributes.empty()) 
    {
        m_Code.AddCode("");
    }
}

void OpenGLShaderDecompiler::DeclareInputAttribute(ShaderAttributeIndex Index) 
{
    uint32_t Location = GetGenericAttributeIndex(Index);
    std::string Name = GetGenericInputAttribute(Index);
    if (m_Stage == ShaderType_Geometry) 
    {
        Name = "gs_" + Name + "[]";
    }

    std::string Suffix;
    if (m_Stage == ShaderType_Fragment)
    {
        ShaderPixelImap InputMode = m_Header.GetPixelImap(Location);
        if (InputMode == ShaderPixelImap_Unused) 
        {
            return;
        }
        Suffix = GetInputFlags(InputMode);
    }

    m_Code.AddCode("layout (location = %d) %s in vec4 %s;", Location, Suffix.c_str(), Name.c_str());
}

void OpenGLShaderDecompiler::DeclareInternalFlags()
{
    for (uint32_t i = 0; i < ShaderInternalFlag_Amount; i++)
    {
        m_Code.AddCode("bool %s = false;", GetInternalFlag((ShaderInternalFlag)i));
    }
    m_Code.AddCode("");
}

void OpenGLShaderDecompiler::DeclareLocalMemory()
{
    uint64_t LocalMemorySize = 0;
    if (m_Stage == ShaderType_Compute) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    } 
    LocalMemorySize = m_Header.GetLocalMemorySize();
    if (LocalMemorySize == 0)
    {
        return;
    }
    m_Code.AddCode("uint lmem[%I64d];", AlignUp(LocalMemorySize, 4) / 4);
    m_Code.AddCode("");
}

void OpenGLShaderDecompiler::DeclareOutputAttribute(ShaderAttributeIndex Index) 
{
    const char * swizzle = "xyzw";
    
    for (uint8_t Element = 0; Element < 4;) 
    {
        std::string xfb = GetTransformFeedbackDecoration(Index, Element);
        if (!xfb.empty()) 
        {
            xfb = stdstr_f(", %s", xfb.c_str());
        }
        uint64_t remainder = 4 - Element;
        VaryingTFBMap::const_iterator itr = m_TransformFeedback.find((uint8_t)((Index * 4) + Element));
        uint64_t NumComponents = itr == m_TransformFeedback.end() ? remainder : itr->second.Components;
        const char * Type = FloatType[NumComponents - 1];
        uint32_t Location = GetGenericAttributeIndex(Index);

        VaryingDescription Description;
        Description.FirstElement = Element;
        Description.IsScalar = NumComponents == 1;
        Description.Name = stdstr_f("out_attr%d", Location);
        if (Element != 0 || NumComponents != 4)
        {
            Description.Name = stdstr_f("%s_%.*s", Description.Name.c_str(), NumComponents, &swizzle[Element]);
        }
        for (uint64_t i = 0; i < NumComponents; ++i) 
        {
            uint8_t Offset = (uint8_t)(Location * 4 + Element + i);
            m_VaryingDescription.insert({Offset, Description});
        }
        m_Code.AddCode("layout (location = %d, component = %d%s) out %s %s;", Location, (uint32_t)Element, xfb.c_str(), Type, Description.Name.c_str());
        Element += (uint8_t)NumComponents;
    }
}

void OpenGLShaderDecompiler::DeclareOutputAttributes()
{
    const ShaderAttributeIndexSet & Attributes = m_ShaderIR.GetOutputAttributes();
    bool DeclatedOutputAttribute = false;
    for (ShaderAttributeIndexSet::const_iterator itr = Attributes.begin(); itr != Attributes.end(); itr++)
    {
        ShaderAttributeIndex Attribute = *itr;
        if (IsGenericAttribute(Attribute)) 
        {
            DeclatedOutputAttribute = true;
            DeclareOutputAttribute(Attribute);
        }
    }
    if (DeclatedOutputAttribute) 
    {
        m_Code.AddCode("");
    }
}

void OpenGLShaderDecompiler::DeclarePhysicalAttributeReader()
{
    if (!m_ShaderIR.HasPhysicalAttributes()) 
    {
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void OpenGLShaderDecompiler::DeclarePredicates()
{
    ShaderIR::PredicateSet Predicates = m_ShaderIR.GetPredicates();
    for (ShaderIR::PredicateSet::const_iterator itr = Predicates.begin(); itr != Predicates.end(); itr++)
    {
        m_Code.AddCode("bool pred%d = false;", *itr);
    }
    if (!Predicates.empty()) 
    {
        m_Code.AddCode("");
    }
}

void OpenGLShaderDecompiler::DeclareRegisters()
{
    const ShaderIR::RegisterSet & Registers = m_ShaderIR.GetRegisters();
    for (ShaderIR::RegisterSet::const_iterator itr = Registers.begin(); itr != Registers.end(); itr++)
    {
        m_Code.AddCode("float gpr%d = 0.0f;", *itr);
    }
    if (!Registers.empty()) 
    {
        m_Code.AddCode("");
    }
}

void OpenGLShaderDecompiler::DeclareSamplers()
{
    uint32_t Binding = m_Device.GetBaseBindings(m_Stage).Sampler;
    const ShaderSamplerEntryList & Samplers = m_ShaderIR.GetSamplers();
    for (ShaderSamplerEntryList::const_iterator itr = Samplers.begin(); itr != Samplers.end(); itr++) 
    {
        const ShaderSamplerEntry & Sampler = *itr;
        std::string Name = stdstr_f("sampler%d",Sampler.Index());
        std::string Description = stdstr_f("layout (binding = %d) uniform", Binding);
        Binding += Sampler.IsIndexed() ? Sampler.Size() : 1;

        std::string SamplerType;
        if (Sampler.IsBuffer()) 
        {
            SamplerType = "samplerBuffer";
        }
        else 
        {
            switch (Sampler.Type()) 
            {
            case ShaderTextureType_1D: SamplerType = "sampler1D"; break;
            case ShaderTextureType_2D: SamplerType = "sampler2D"; break;
            case ShaderTextureType_3D: SamplerType = "sampler3D"; break;
            case ShaderTextureType_Cube: SamplerType = "samplerCube"; break;
            default:
                g_Notify->BreakPoint(__FILE__, __LINE__);
                SamplerType = "sampler2D";
            }
        }
        if (Sampler.IsArray()) 
        {
            SamplerType += "Array";
        }
        if (Sampler.IsShadow()) 
        {
            SamplerType += "Shadow";
        }
        if (!Sampler.IsIndexed())
        {
            m_Code.AddCode("%s %s %s;", Description.c_str(), SamplerType.c_str(), Name.c_str());
        } 
        else 
        {
            m_Code.AddCode("%s %s %s[%d];", Description.c_str(), SamplerType.c_str(), Name.c_str(), Sampler.Size());
        }
    }
    if (!m_ShaderIR.GetSamplers().empty()) 
    {
        m_Code.AddCode("");
    }
}

void OpenGLShaderDecompiler::DeclareVertex() 
{
    if (m_Stage != ShaderType_Vertex)
    {
        return;
    }

    m_Code.AddCode("out gl_PerVertex {");
    m_Code.ScopeIncrease();

    std::string Position = GetTransformFeedbackDecoration(ShaderAttributeIndex_Position);
    if (!Position.empty())
    {
        Position = stdstr_f("layout (%s) ", Position.c_str());
    }
    VaryingTFBMap::const_iterator TFBItr = m_TransformFeedback.find(ShaderAttributeIndex_Position * 4);
    const char * PositionType = FloatType[(TFBItr != m_TransformFeedback.end() ? TFBItr->second.Components : 4) - 1];
    m_Code.AddCode("%s%s gl_Position;", Position.c_str(), PositionType);

    const ShaderAttributeIndexSet & Attributes = m_ShaderIR.GetOutputAttributes();
    for (ShaderAttributeIndexSet::const_iterator itr = Attributes.begin(); itr != Attributes.end(); itr++)
    {
        ShaderAttributeIndex Attribute = *itr;
        if (Attribute == ShaderAttributeIndex_ClipDistances0123 ||
            Attribute == ShaderAttributeIndex_ClipDistances4567)
        {
            m_Code.AddCode("float gl_ClipDistance[];");
            break;
        }
    }

    m_Code.ScopeDecrease();
    m_Code.AddCode("};");
    m_Code.AddCode("");

    if (m_Stage == ShaderType_Geometry) 
    {
        m_Code.AddCode("");
    }
}

OpenGLShaderDecompiler::VaryingTFBMap OpenGLShaderDecompiler::BuildTransformFeedback(const ShaderGraphicsInfo & Info)
{
    int Vectors[] = 
    {
        28,  // gl_Position
        32,  // Generic 0
        36,  // Generic 1
        40,  // Generic 2
        44,  // Generic 3
        48,  // Generic 4
        52,  // Generic 5
        56,  // Generic 6
        60,  // Generic 7
        64,  // Generic 8
        68,  // Generic 9
        72,  // Generic 10
        76,  // Generic 11
        80,  // Generic 12
        84,  // Generic 13
        88,  // Generic 14
        92,  // Generic 15
        96,  // Generic 16
        100, // Generic 17
        104, // Generic 18
        108, // Generic 19
        112, // Generic 20
        116, // Generic 21
        120, // Generic 22
        124, // Generic 23
        128, // Generic 24
        132, // Generic 25
        136, // Generic 26
        140, // Generic 27
        144, // Generic 28
        148, // Generic 29
        152, // Generic 30
        156, // Generic 31
        160, // gl_FrontColor
        164, // gl_FrontSecondaryColor
        160, // gl_BackColor
        164, // gl_BackSecondaryColor
        192, // gl_TexCoord[0]
        196, // gl_TexCoord[1]
        200, // gl_TexCoord[2]
        204, // gl_TexCoord[3]
        208, // gl_TexCoord[4]
        212, // gl_TexCoord[5]
        216, // gl_TexCoord[6]
        220, // gl_TexCoord[7]
    };
    VaryingTFBMap TransformFeedback;

    for (uint32_t i = 0; i < CMaxwell3D::NumTransformFeedbackBuffers; i++) 
    {
        const CMaxwell3D::tyTransformFeedbackLayout & Layout = Info.TFBLayouts[i];
        const uint8_t * Locations = Info.TFBVaryingLocs[i];
        uint32_t VaryingCount = Layout.VaryingCount;

        uint32_t Highest = 0;
        for (uint32_t Offset = 0; Offset < VaryingCount; Offset++) 
        {
            uint32_t BaseOffset = Offset;
            uint8_t Location = Locations[Offset];

            VaryingTFB varying;
            varying.Buffer = Layout.Stream;
            varying.Stride = Layout.Stride;
            varying.Offset = Offset * sizeof(uint32_t);
            varying.Components = 1;

            if (std::find(std::begin(Vectors), std::end(Vectors), Location / 4 * 4) != std::end(Vectors)) 
            {
                if (Location % 4 != 0)
                {
                    g_Notify->BreakPoint(__FILE__, __LINE__);
                }

                uint8_t BaseIndex = Location / 4;
                while (Offset + 1 < VaryingCount && BaseIndex == Locations[Offset + 1] / 4) 
                {
                    Offset += 1;
                    varying.Components += 1;
                }
            }

            bool Inserted = TransformFeedback.emplace(Location, varying).second;
            if (!Inserted) 
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }

            Highest = std::max(Highest, (uint32_t)((BaseOffset + varying.Components) * sizeof(uint32_t)));
        }

        if (Highest != Layout.Stride)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    return TransformFeedback;
}

OpenGLExpression OpenGLShaderDecompiler::ApplyPrecise(const ShaderOperationNode & Operation, const char * Value, OpenGLExpressionType Type)
{
    if (!Operation.IsPrecise()) 
    {
        return OpenGLExpression(Value, Type);
    }
    bool DisablePrecise = m_Device.HasPreciseBug() && m_Stage == ShaderType_Fragment;
    std::string TemporaryId = m_Code.TemporaryId();
    m_Code.AddCode("%s%s %s = %s;", DisablePrecise ? "" : "precise ", GetTypeString(Type), TemporaryId.c_str(), Value);
    return OpenGLExpression(TemporaryId.c_str(), Type);
}

OpenGLExpression OpenGLShaderDecompiler::GetOutputAttribute(const ShaderAttributeBufferNode * AttributeBuffer) 
{
    uint32_t Element = AttributeBuffer->GetElement();
    ShaderAttributeIndex AttributeIndex = AttributeBuffer->GetIndex();

    switch (AttributeIndex) 
    {
    case ShaderAttributeIndex_Position: return OpenGLExpression(stdstr_f("gl_Position%s",GetSwizzle(Element)).c_str(), OpenGLExpressionType_Float);
    }

    if (IsGenericAttribute(AttributeIndex)) 
    {
        uint8_t Offset = (uint8_t)(GetGenericAttributeIndex(AttributeIndex) * 4 + Element);
        const VaryingDescription & Description = m_VaryingDescription.at(Offset);
        std::string OutputAttribute = Description.IsScalar ? Description.Name : stdstr_f("%s[%d]", Description.Name.c_str(), Element - Description.FirstElement);
        return OpenGLExpression(OutputAttribute.c_str(), OpenGLExpressionType_Float);
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return OpenGLExpression();
}

OpenGLExpression OpenGLShaderDecompiler::ReadAttribute(ShaderAttributeIndex Attribute,  uint32_t Element, const ShaderNodePtr & Buffer)
{
    switch (Attribute)
    {
    case ShaderAttributeIndex_Position:
        switch (m_Stage) 
        {
        case ShaderType_Fragment: return {stdstr_f("gl_FragCoord%s", GetSwizzle(Element)).c_str(), OpenGLExpressionType_Float};
        }
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return {"0", OpenGLExpressionType_Int};
    }

    if (IsGenericAttribute(Attribute))
    {
        if (m_Stage == ShaderType_Geometry && Buffer.get() != nullptr)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return OpenGLExpression("0", OpenGLExpressionType_Int);
        }
        return OpenGLExpression(stdstr_f("%s%s",GetGenericInputAttribute(Attribute).c_str(),GetSwizzle(Element)).c_str(), OpenGLExpressionType_Float);
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return OpenGLExpression("0", OpenGLExpressionType_Int);
}

OpenGLExpression OpenGLShaderDecompiler::Visit(const ShaderNodePtr & Node)
{
    ShaderNodeType NodeType = Node->GetShaderNodeType();
    if (NodeType == ShaderNodeType_Operation)
    {
        ShaderOperationNode * Operation = Node->GetShaderOperationNode();
        ShaderOperationCode OperationCode = Operation->GetCode();
        if (OperationCode >= sizeof(m_OperationDecompilers) / sizeof(m_OperationDecompilers[0]))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return OpenGLExpression();
        }
        const fnOperationDecompiler Decompiler = m_OperationDecompilers[OperationCode];
        if (Decompiler == nullptr)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return OpenGLExpression();
        }
        return (this->*Decompiler)(*Operation);
    }
    else if (NodeType == ShaderNodeType_GeneralPurposeRegister) 
    {
        const ShaderGprNode * Gpr = Node->GetShaderGprNode();
        uint32_t Index = Gpr->GetIndex();
        if (Index == ShaderRegister_ZeroIndex)
        {
            return OpenGLExpression("0U", OpenGLExpressionType_Uint);
        }
        return OpenGLExpression(stdstr_f("gpr%d",Index).c_str(), OpenGLExpressionType_Float);
    }
    else if (NodeType == ShaderNodeType_Immediate) 
    {
        const ShaderImmediateNode* ImmediateNode = Node->GetShaderImmediateNode();
        uint32_t Value = ImmediateNode->GetValue();
        return OpenGLExpression(stdstr_f(Value < 10 ? "%dU" : "0x%XU", Value).c_str(), OpenGLExpressionType_Uint);
    }
    else if (NodeType == ShaderNodeType_Predicate)
    {
        const ShaderPredicateNode * PredicateNode = Node->GetShaderPredicateNode();
        std::string Value;
        switch (PredicateNode->GetPredicate())
        {
        case ShaderPredicate_UnusedIndex: Value = "true"; break;
        case ShaderPredicate_NeverExecute: Value = "false"; break;
        default: Value = stdstr_f("pred%d", PredicateNode->GetPredicate()); break;
        }
        if (PredicateNode->IsNegated()) 
        {
            return OpenGLExpression(stdstr_f("!(%s)", Value.c_str()).c_str(), OpenGLExpressionType_Bool);
        }
        return OpenGLExpression(Value.c_str(), OpenGLExpressionType_Bool);
    }
    else if (NodeType == ShaderNodeType_AttributeBufferMemory)
    {
        const ShaderAttributeBufferNode * AttributeBuffer = Node->GetShaderAttributeBufferNode();
        if (AttributeBuffer->IsPhysicalBuffer() && m_Stage == ShaderType_Geometry) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return OpenGLExpression();
        }
        if (AttributeBuffer->IsPhysicalBuffer()) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return OpenGLExpression();
        }
        return ReadAttribute(AttributeBuffer->GetIndex(), AttributeBuffer->GetElement(), AttributeBuffer->GetBuffer());
    } 
    else if (NodeType == ShaderNodeType_ConstantBuffer)
    {
        const ShaderCbufNode * CbufNode = Node->GetShaderCbufNode();
        const ShaderNodePtr Offset = CbufNode->GetOffset();
        if (Offset->GetShaderNodeType() == ShaderNodeType_Immediate) 
        {
            const ShaderImmediateNode * ImmediateNode = Offset->GetShaderImmediateNode();
            uint32_t ImmediateValue = ImmediateNode->GetValue();
            if (ImmediateValue % 4 != 0) 
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
            if (m_UseUnifiedUniforms)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return OpenGLExpression();
            } 
            return OpenGLExpression(stdstr_f("cbuf%d[%d][%d]", CbufNode->GetIndex(),ImmediateValue / (4 * 4), (ImmediateValue / 4) % 4).c_str(),OpenGLExpressionType_Uint);
        }
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return OpenGLExpression();
    }
    else if (NodeType == ShaderNodeType_Comment)
    {
        ShaderCommentNode * Comment = Node->GetShaderCommentNode();
        m_Code.AddCode("// %s", Comment->GetText());
        return OpenGLExpression();
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return OpenGLExpression();
}

OpenGLExpression OpenGLShaderDecompiler::VisitOperand(const ShaderOperationNode & Operation, uint32_t OperandIndex) 
{
    const ShaderNodePtr & Operand = Operation.GetOperand(OperandIndex);
    if (!Operation.IsPrecise() || IsPrecise(Operand) || Operand->GetShaderNodeType() != ShaderNodeType_Operation) 
    {
        return Visit(Operand);
    }

    OpenGLExpression value = Visit(Operand);
    std::string TemporaryId = m_Code.TemporaryId();
    m_Code.AddCode("%s %s = %s;", GetTypeString(value.GetType()), TemporaryId.c_str(), value.GetCode());
    return OpenGLExpression(TemporaryId.c_str(), value.GetType());
}
    
OpenGLExpression OpenGLShaderDecompiler::Assign(const ShaderOperationNode & Operation) 
{
    const ShaderNodePtr & Dest = Operation.GetOperand(0);
    const ShaderNodePtr & Src = Operation.GetOperand(1);

    OpenGLExpression Target;
    if (Dest->GetShaderNodeType() == ShaderNodeType_GeneralPurposeRegister)
    {
        const ShaderGprNode * GPR = Dest->GetShaderGprNode();
        if (GPR->GetIndex() == ShaderRegister_ZeroIndex)
        {
            m_Code.AddCode("%s;", Visit(Src).GetCode());
            return OpenGLExpression();
        }
        Target = OpenGLExpression(stdstr_f("gpr%d",GPR->GetIndex()).c_str(), OpenGLExpressionType_Float);
    }
    else if (Dest->GetShaderNodeType() == ShaderNodeType_AttributeBufferMemory) 
    {
        const ShaderAttributeBufferNode * AttributeBuffer = Dest->GetShaderAttributeBufferNode();
        if (AttributeBuffer->IsPhysicalBuffer())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        Target = GetOutputAttribute(AttributeBuffer);
    } 
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_Code.AddCode("%s = %s;", Target.GetCode(), Visit(Src).As(Target.GetType()).c_str());
    return OpenGLExpression();
}

OpenGLExpression OpenGLShaderDecompiler::Exit(const ShaderOperationNode & /*Operation*/)
{
    if (m_Stage == ShaderType_Fragment)
    {
        if (m_Header.PS.OmapSampleMask != 0) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }

        const ShaderIR::RegisterSet & UsedRegisters = m_ShaderIR.GetRegisters();
        uint32_t CurrentReg = 0;
        for (uint32_t i = 0; i < NumRenderTargets; i++) 
        {
            for (uint32_t Component = 0; Component < 4; Component++)
            {
                if (m_Header.IsColorComponentOutputEnabled(i, Component))
                {
                    OpenGLExpression SafeRegister = UsedRegisters.find(CurrentReg) != UsedRegisters.end() ? OpenGLExpression(stdstr_f("gpr%d",CurrentReg).c_str(), OpenGLExpressionType_Float) : OpenGLExpression("0.0f", OpenGLExpressionType_Float);
                    const char * Swizzle[] = { ".r", ".g", ".b", ".a" };                    
                    m_Code.AddCode("frag_color%d%s = %s;", i, Swizzle[Component], SafeRegister.AsFloat().c_str());
                    CurrentReg += 1;
                }
            }
        }

        if (m_Header.PS.OmapDepth) 
        {
            OpenGLExpression SafeRegister = UsedRegisters.find(CurrentReg + 1) != UsedRegisters.end() ? OpenGLExpression(stdstr_f("gpr%d",CurrentReg + 1).c_str(), OpenGLExpressionType_Float) : OpenGLExpression("0.0f", OpenGLExpressionType_Float);
            m_Code.AddCode("gl_FragDepth = %s;", SafeRegister.AsFloat().c_str());
        }
    }
    m_Code.AddCode("return;");
    return OpenGLExpression();
}

OpenGLExpression OpenGLShaderDecompiler::FAdd(const ShaderOperationNode & Operation) 
{
    stdstr_f OpStr("(%s + %s)", VisitOperand(Operation, 0).As(OpenGLExpressionType_Float).c_str(), VisitOperand(Operation, 1).As(OpenGLExpressionType_Float).c_str());
    return ApplyPrecise(Operation, OpStr.c_str(), OpenGLExpressionType_Float);
}

OpenGLExpression OpenGLShaderDecompiler::FDiv(const ShaderOperationNode & Operation) 
{
    stdstr_f OpStr("(%s / %s)", VisitOperand(Operation, 0).As(OpenGLExpressionType_Float).c_str(), VisitOperand(Operation, 1).As(OpenGLExpressionType_Float).c_str());
    return ApplyPrecise(Operation, OpStr.c_str(), OpenGLExpressionType_Float);
}

OpenGLExpression OpenGLShaderDecompiler::FFloor(const ShaderOperationNode & Operation) 
{
    stdstr_f OpStr("floor(%s)", VisitOperand(Operation, 0).As(OpenGLExpressionType_Float).c_str());
    return ApplyPrecise(Operation, OpStr.c_str(), OpenGLExpressionType_Float);
}

OpenGLExpression OpenGLShaderDecompiler::FFma(const ShaderOperationNode & Operation) 
{
    stdstr_f OpStr("fma(%s, %s, %s)", VisitOperand(Operation, 0).As(OpenGLExpressionType_Float).c_str(), VisitOperand(Operation, 1).As(OpenGLExpressionType_Float).c_str(), VisitOperand(Operation, 2).As(OpenGLExpressionType_Float).c_str());
    return ApplyPrecise(Operation, OpStr.c_str(), OpenGLExpressionType_Float);
}

OpenGLExpression OpenGLShaderDecompiler::FMax(const ShaderOperationNode & Operation) 
{
    stdstr_f OpStr("max(%s, %s)", VisitOperand(Operation, 0).As(OpenGLExpressionType_Float).c_str(), VisitOperand(Operation, 1).As(OpenGLExpressionType_Float).c_str());
    return ApplyPrecise(Operation, OpStr.c_str(), OpenGLExpressionType_Float);
}

OpenGLExpression OpenGLShaderDecompiler::FMin(const ShaderOperationNode & Operation) 
{
    stdstr_f OpStr("min(%s, %s)", VisitOperand(Operation, 0).As(OpenGLExpressionType_Float).c_str(), VisitOperand(Operation, 1).As(OpenGLExpressionType_Float).c_str());
    return ApplyPrecise(Operation, OpStr.c_str(), OpenGLExpressionType_Float);
}

OpenGLExpression OpenGLShaderDecompiler::FMul(const ShaderOperationNode & Operation) 
{
    stdstr_f OpStr("(%s * %s)", VisitOperand(Operation, 0).As(OpenGLExpressionType_Float).c_str(), VisitOperand(Operation, 1).As(OpenGLExpressionType_Float).c_str());
    return ApplyPrecise(Operation, OpStr.c_str(), OpenGLExpressionType_Float);
}

OpenGLExpression OpenGLShaderDecompiler::FNegate(const ShaderOperationNode & Operation)
{
    stdstr_f OpStr("-(%s)", VisitOperand(Operation, 0).As(OpenGLExpressionType_Float).c_str());
    return ApplyPrecise(Operation, OpStr.c_str(), OpenGLExpressionType_Float);
}

OpenGLExpression OpenGLShaderDecompiler::Select(const ShaderOperationNode & Operation) 
{
    stdstr_f OpStr("(%s ? %s : %s)", Visit(Operation.GetOperand(0)).AsBool().c_str(), Visit(Operation.GetOperand(1)).AsUint().c_str(), Visit(Operation.GetOperand(2)).AsUint().c_str());
    return ApplyPrecise(Operation, OpStr.c_str(), OpenGLExpressionType_Uint);
}

OpenGLExpression OpenGLShaderDecompiler::Texture(const ShaderOperationNode & Operation)
{
    const ShaderTexture & Texture = Operation.GetTexture();
    const ShaderSamplerEntry & Sampler = Texture.Sampler();
    bool SeparateDC = Sampler.Type() == ShaderTextureType_Cube && Sampler.IsArray() && Sampler.IsShadow();
    std::string TextureCode = GetTextureCode(Operation, OpenGLExpressionType_Float, Texture.Bias(), true, SeparateDC);
    if (Sampler.IsShadow()) 
    {
        TextureCode = stdstr_f("vec4(%s)", TextureCode.c_str());
    }
    TextureCode += GetSwizzle(Texture.Element());
    return OpenGLExpression(TextureCode.c_str(), OpenGLExpressionType_Float);
}

bool OpenGLShaderDecompiler::UseUnifiedUniforms(const OpenGLDevice & Device, const ShaderIR & ShaderIR, ShaderType Stage) 
{
    uint32_t NumUniformBuffers = (uint32_t)ShaderIR.GetConstantBuffers().size();
    uint32_t NumAvailableUniformBuffers = Device.GetMaxUniformBuffers(Stage) - 1;
    return NumUniformBuffers > NumAvailableUniformBuffers;
}

bool OpenGLShaderDecompiler::IsGenericAttribute(ShaderAttributeIndex Index) 
{
    if (Index >= ShaderAttributeIndex_Attribute_0 && Index <= ShaderAttributeIndex_Attribute_31)
    {
        return true;
    }
    return false;
}

const char * OpenGLShaderDecompiler::GetInputFlags(ShaderPixelImap attribute) 
{
    switch (attribute) 
    {
    case ShaderPixelImap_Perspective: return "smooth";
    case ShaderPixelImap_Constant: return "flat";
    case ShaderPixelImap_ScreenLinear: return "noperspective";
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return "";
}

uint32_t OpenGLShaderDecompiler::GetGenericAttributeIndex(ShaderAttributeIndex Index) 
{
    if (!IsGenericAttribute(Index))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return Index - ShaderAttributeIndex_Attribute_0;
}

std::string OpenGLShaderDecompiler::GetGenericInputAttribute(ShaderAttributeIndex Attribute)
{
    return stdstr_f("in_attr%d", GetGenericAttributeIndex(Attribute));
}

const char * OpenGLShaderDecompiler::GetInternalFlag(ShaderInternalFlag Flag) 
{
    switch (Flag) 
    { 
    case ShaderInternalFlag_Zero: return "zero_flag";
    case ShaderInternalFlag_Sign: return "sign_flag";
    case ShaderInternalFlag_Carry: return "carry_flag";
    case ShaderInternalFlag_Overflow: return "overflow_flag";
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return "unknown_flag";
}

const char * OpenGLShaderDecompiler::GetTypeString(OpenGLExpressionType Type) 
{
    switch (Type) 
    {
    case OpenGLExpressionType_Float: return "float";
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return "unknown";
}

const char * OpenGLShaderDecompiler::GetSwizzle(uint32_t Element)
{
    switch (Element)
    {
    case 0: return ".x";
    case 1: return ".y";
    case 2: return ".z";
    case 3: return ".w";
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return "";
}

bool OpenGLShaderDecompiler::IsPrecise(const ShaderNodePtr & Node) 
{
    if (Node->GetShaderNodeType() == ShaderNodeType_Operation) 
    {
        Node->GetShaderOperationNode()->IsPrecise();
    }
    return false;
}
