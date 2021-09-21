#include "ShaderNode.h"
#include "VideoNotification.h"

ShaderNode::ShaderNode() :
    m_ref(0)
{
}

ShaderCbufNode * ShaderNode::GetShaderCbufNode(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

ShaderCommentNode * ShaderNode::GetShaderCommentNode(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

ShaderImmediateNode * ShaderNode::GetShaderImmediateNode(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

ShaderOperationNode * ShaderNode::GetShaderOperationNode(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

ShaderGprNode * ShaderNode::GetShaderGprNode(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

ShaderAttributeBufferNode * ShaderNode::GetShaderAttributeBufferNode(void) 
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

ShaderPredicateNode * ShaderNode::GetShaderPredicateNode(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

ShaderNodePtr::ShaderNodePtr() :
    m_Object(nullptr)
{
}

ShaderNodePtr::ShaderNodePtr(ShaderNode * Object) :
    m_Object(Object)
{
    if (m_Object)
    {
        m_Object->m_ref += 1;
    }
}

ShaderNodePtr::ShaderNodePtr(const ShaderNodePtr & rhs) :
    m_Object(rhs.m_Object)
{
    if (m_Object)
    {
        m_Object->m_ref += 1;
    }
}

ShaderNodePtr & ShaderNodePtr::operator=(const ShaderNodePtr& rhs)
{
    if (m_Object) 
    {
        ReleaseRef();
    }
    m_Object = rhs.m_Object;
    if (m_Object)
    {
        m_Object->m_ref += 1;
    }
    return *this;
}

ShaderNodePtr::~ShaderNodePtr()
{
    ReleaseRef();
}

ShaderNode * ShaderNodePtr::get() const
{
    return m_Object;
}

void ShaderNodePtr::Reset(ShaderNode * Object) 
{
    if (Object != m_Object) 
    {
        ReleaseRef();
        m_Object = Object;
        if (m_Object != nullptr) 
        {
            m_Object->m_ref += 1;
        }
    }
}

void ShaderNodePtr::ReleaseRef() 
{
    if (m_Object == nullptr) 
    {
        return;
    }
    m_Object->m_ref -= 1;
    if (m_Object->m_ref == 0) 
    {
        switch (m_Object->GetShaderNodeType()) 
        {
        case ShaderNodeType_Comment: 
            delete m_Object->GetShaderCommentNode(); 
            break;
        case ShaderNodeType_ConstantBuffer:
            delete m_Object->GetShaderCbufNode();
            break;
        case ShaderNodeType_Immediate:
            delete m_Object->GetShaderImmediateNode();
            break;
        case ShaderNodeType_GeneralPurposeRegister:
            delete m_Object->GetShaderGprNode();
            break;
        case ShaderNodeType_Operation:
            delete m_Object->GetShaderOperationNode();
            break;
        case ShaderNodeType_AttributeBufferMemory:
            delete m_Object->GetShaderAttributeBufferNode();
            break;
        case ShaderNodeType_Predicate:
            delete m_Object->GetShaderPredicateNode();
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
}

ShaderCbufNode::ShaderCbufNode(uint32_t Index, ShaderNodePtr Offset) :
    m_Index(Index),
    m_Offset(Offset)
{
}

ShaderCommentNode::ShaderCommentNode(const char * Text) :
    m_Text(Text)
{
}

ShaderCommentNode::~ShaderCommentNode()
{
}

ShaderGprNode::ShaderGprNode(uint32_t Index) :
    m_Index(Index) 
{
}

ShaderGprNode::~ShaderGprNode()
{
}

ShaderAttributeBufferNode::ShaderAttributeBufferNode(ShaderAttributeIndex Index, uint32_t Eement, ShaderNodePtr Buffer) : 
    m_Buffer(Buffer),
    m_Index(Index),
    m_Element(Eement)
{
}

ShaderImmediateNode::ShaderImmediateNode(int32_t Value) :
    m_Value((uint32_t)Value)
{
}

ShaderImmediateNode::ShaderImmediateNode(uint32_t Value) :
    m_Value(Value)
{
}

ShaderImmediateNode::ShaderImmediateNode(float Value)
{
    std::memcpy(&m_Value, &Value, sizeof(uint32_t));
}

ShaderPredicateNode::ShaderPredicateNode(ShaderPredicate Predicate, bool Negated) :
    m_Predicate(Predicate),
    m_Negated(Negated)
{
}

ShaderSamplerEntry::ShaderSamplerEntry() :
    m_Type(ShaderTextureType_1D),
    m_Index(0),
    m_Offset(0),
    m_Size(1),
    m_IsArray(false), 
    m_IsShadow(false), 
    m_IsBuffer(false), 
    m_IsBindless(false),
    m_IsIndexed(false)
{
}

ShaderSamplerEntry::ShaderSamplerEntry(uint32_t Index, uint32_t Offset, ShaderTextureType Type, bool IsArray, bool IsShadow, bool IsBuffer, bool IsIndexed) : 
    m_Type(Type),
    m_Index(Index),
    m_Offset(Offset),
    m_Size(1),
    m_IsArray(IsArray), 
    m_IsShadow(IsShadow), 
    m_IsBuffer(IsBuffer), 
    m_IsBindless(false),
    m_IsIndexed(IsIndexed)
{
}

ShaderSamplerEntry::ShaderSamplerEntry(ShaderSamplerEntry const & Entry) : 
    m_Type(Entry.m_Type),
    m_Index(Entry.m_Index),
    m_Offset(Entry.m_Offset),
    m_Size(Entry.m_Size),
    m_IsArray(Entry.m_IsArray),
    m_IsShadow(Entry.m_IsShadow),
    m_IsBuffer(Entry.m_IsBuffer),
    m_IsBindless(Entry.m_IsBindless),
    m_IsIndexed(Entry.m_IsIndexed)
{
}

ShaderTexture::ShaderTexture() : 
    m_Element(0) 
{
}

ShaderTexture::ShaderTexture(ShaderSamplerEntry Sampler, ShaderNodePtr Array, ShaderNodePtr DepthCompare, ShaderNodeList AOffi, ShaderNodeList PTP, ShaderNodeList Derivates, ShaderNodePtr Bias, ShaderNodePtr Lod, ShaderNodePtr Component, uint32_t Element, ShaderNodePtr Index) :
    m_Sampler(Sampler),
    m_Array(Array),
    m_DepthCompare(DepthCompare),
    m_AOffi(AOffi),
    m_Derivates(Derivates),
    m_PTP(PTP),
    m_Bias(Bias),
    m_Lod(Lod),
    m_Index(Index),
    m_Component(Component),
    m_Element(Element)
{
}

ShaderTexture::ShaderTexture(const ShaderTexture & Texture) :
    m_Sampler(Texture.m_Sampler),
    m_Array(Texture.m_Array),
    m_DepthCompare(Texture.m_DepthCompare),
    m_AOffi(Texture.m_AOffi),
    m_Derivates(Texture.m_Derivates),
    m_PTP(Texture.m_PTP),
    m_Bias(Texture.m_Bias),
    m_Lod(Texture.m_Lod),
    m_Index(Texture.m_Index),
    m_Component(Texture.m_Component),
    m_Element(Texture.m_Element)
{
}

ShaderOperationNode::ShaderOperationNode(ShaderOperationCode Code) : 
    m_OperationType(ShaderOperationType_Unknown),
    m_Code(Code),
    m_Precise(false) 
{
}

ShaderOperationNode::ShaderOperationNode(ShaderOperationCode Code, ShaderTexture Texture, ShaderNodeList Operands) : 
    m_Code(Code), 
    m_OperationType(ShaderOperationType_Texture),
    m_Texture(std::move(Texture)),
    m_Operands(std::move(Operands)),
    m_Precise(false) 
{
}

ShaderOperationNode::ShaderOperationNode(ShaderOperationCode Code, bool Precise, ShaderNodeList Operands):
    m_Code(Code), 
    m_OperationType(ShaderOperationType_Operand), 
    m_Operands(std::move(Operands)),
    m_Precise(Precise) 
{
}
