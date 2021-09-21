#pragma once
#include "ShaderInstruction.h"
#include "Shader.h"
#include <vector>
#include <string>

enum ShaderNodeType 
{
    ShaderNodeType_Unknown,
    ShaderNodeType_AttributeBufferMemory,
    ShaderNodeType_Comment,
    ShaderNodeType_ConstantBuffer,
    ShaderNodeType_Immediate,
    ShaderNodeType_GeneralPurposeRegister,
    ShaderNodeType_Operation,
    ShaderNodeType_Predicate,
};

enum ShaderOperationCode
{
    ShaderOperationCode_Assign,
    ShaderOperationCode_Select,
    ShaderOperationCode_FAdd,
    ShaderOperationCode_FMul,
    ShaderOperationCode_FDiv,
    ShaderOperationCode_FFma,
    ShaderOperationCode_FNegate,
    ShaderOperationCode_FAbsolute,
    ShaderOperationCode_FMin,
    ShaderOperationCode_FMax,
    ShaderOperationCode_FFloor,
    ShaderOperationCode_LogicalAssign,
    ShaderOperationCode_LogicalFOrdEqual,
    ShaderOperationCode_Texture,
    ShaderOperationCode_TextureLod,
    ShaderOperationCode_Branch,
    ShaderOperationCode_Exit,
    ShaderOperationCode_Discard,
    ShaderOperationCode_Amount,
};

class ShaderAttributeBufferNode;
class ShaderCbufNode;
class ShaderNodePtr;
class ShaderCommentNode;
class ShaderImmediateNode;
class ShaderOperationNode;
class ShaderGprNode;
class ShaderPredicateNode;

class ShaderNode 
{
    friend ShaderNodePtr;

protected:
    ShaderNode(void);

public:
    virtual ShaderNodeType GetShaderNodeType() const = 0;
    virtual ShaderAttributeBufferNode * GetShaderAttributeBufferNode(void);
    virtual ShaderCbufNode * GetShaderCbufNode(void);
    virtual ShaderCommentNode * GetShaderCommentNode(void);
    virtual ShaderImmediateNode * GetShaderImmediateNode(void);
    virtual ShaderGprNode * GetShaderGprNode(void);
    virtual ShaderOperationNode * GetShaderOperationNode(void);
    virtual ShaderPredicateNode * GetShaderPredicateNode(void);

private:
    ShaderNode(const ShaderNode&);
    ShaderNode& operator=(const ShaderNode&);

    uint32_t m_ref;
};

class ShaderNodePtr 
{
public:
    ShaderNodePtr();
    ShaderNodePtr(ShaderNode * Object);
    ShaderNodePtr(const ShaderNodePtr&);
    ShaderNodePtr& operator=(const ShaderNodePtr&);
    ~ShaderNodePtr();

    ShaderNode * get() const;
    ShaderNode * operator->() const;
    void Reset(ShaderNode * Object);
    
    operator ShaderNode *() { return m_Object; }

private:
    void ReleaseRef();

    ShaderNode * m_Object;
};

typedef std::vector<ShaderNodePtr> ShaderNodeList;

class ShaderCbufNode : 
    public ShaderNode
{
public:
    virtual ShaderNodeType GetShaderNodeType() const { return ShaderNodeType_ConstantBuffer; }
    ShaderCbufNode * GetShaderCbufNode(void) { return this; }

    ShaderCbufNode(uint32_t Index, ShaderNodePtr Offset);

    uint32_t GetIndex() const { return m_Index; }
    const ShaderNodePtr & GetOffset() const { return m_Offset; }

private:
    ShaderCbufNode();
    ShaderCbufNode(const ShaderCbufNode&);
    ShaderCbufNode& operator=(const ShaderCbufNode&);

    uint32_t m_Index;
    ShaderNodePtr m_Offset;
};

class ShaderCommentNode :
    public ShaderNode
{
public:
    ShaderNodeType GetShaderNodeType() const { return ShaderNodeType_Comment; }
    ShaderCommentNode * GetShaderCommentNode(void) { return this; }

    ShaderCommentNode(const char* Text);
    ~ShaderCommentNode();

    const char * GetText() const { return m_Text.c_str(); }

private:
    ShaderCommentNode();
    ShaderCommentNode(const ShaderCommentNode&);
    ShaderCommentNode& operator=(const ShaderCommentNode&);

    std::string m_Text;
};

class ShaderGprNode :
    public ShaderNode
{
public:
    ShaderNodeType GetShaderNodeType() const { return ShaderNodeType_GeneralPurposeRegister; }
    ShaderGprNode * GetShaderGprNode(void) { return this; }

    ShaderGprNode(uint32_t Index);
    ~ShaderGprNode();

    uint32_t GetIndex() const;

private:
    ShaderGprNode();
    ShaderGprNode(const ShaderGprNode&);
    ShaderGprNode& operator=(const ShaderGprNode&);

    uint32_t m_Index;
};

class ShaderAttributeBufferNode :
    public ShaderNode
{
public:
    virtual ShaderNodeType GetShaderNodeType() const { return ShaderNodeType_AttributeBufferMemory; }
    ShaderAttributeBufferNode * GetShaderAttributeBufferNode(void) { return this; }

    ShaderAttributeBufferNode(ShaderAttributeIndex Index, uint32_t Eement, ShaderNodePtr Buffer);
    ShaderAttributeBufferNode(ShaderNodePtr PhysicalAaddress, ShaderNodePtr Buffer);

    ShaderAttributeIndex GetIndex() const { return m_Index; }
    uint32_t GetElement() const { return m_Element; }
    const ShaderNodePtr& GetBuffer() const { return m_Buffer; }
    bool IsPhysicalBuffer() const { return m_PhysicalAaddress.get() != nullptr; }
    const ShaderNodePtr & GetPhysicalAddress() const { return m_PhysicalAaddress; }

private:
    ShaderNodePtr m_PhysicalAaddress;
    ShaderNodePtr m_Buffer;
    ShaderAttributeIndex m_Index;
    uint32_t m_Element;
};

class ShaderImmediateNode : 
    public ShaderNode
{
public:
    ShaderNodeType GetShaderNodeType() const { return ShaderNodeType_Immediate; }
    ShaderImmediateNode * GetShaderImmediateNode(void) { return this; }

    ShaderImmediateNode(int32_t Value);
    ShaderImmediateNode(uint32_t Value);
    ShaderImmediateNode(float Value);

    uint32_t GetValue() const { return m_Value; }

private:
    ShaderImmediateNode();
    ShaderImmediateNode(const ShaderImmediateNode&);
    ShaderImmediateNode& operator=(const ShaderImmediateNode&);

    uint32_t m_Value;
};

class ShaderPredicateNode :
    public ShaderNode 
{
public:
    ShaderNodeType GetShaderNodeType() const { return ShaderNodeType_Predicate; }
    ShaderPredicateNode* GetShaderPredicateNode(void) {  return this; }

    ShaderPredicateNode(ShaderPredicate Predicate, bool Negated);
    
    ShaderPredicate GetPredicate() const { return m_Predicate;  }
    bool IsNegated() const { return m_Negated; }

private:
    ShaderPredicateNode();
    ShaderPredicateNode(const ShaderPredicateNode&);
    ShaderPredicateNode& operator=(const ShaderPredicateNode&);

    ShaderPredicate m_Predicate;
    bool m_Negated;
};

class ShaderSamplerEntry 
{
public:
    ShaderSamplerEntry();
    ShaderSamplerEntry(uint32_t Index, uint32_t Offset, ShaderTextureType Type, bool IsArray, bool IsShadow, bool IsBuffer, bool IsIndexed);
    ShaderSamplerEntry(const ShaderSamplerEntry&);

    ShaderTextureType Type(void) const { return m_Type; }
    uint32_t Size(void) const { return m_Size; }
    uint32_t Index(void) const { return m_Index; }
    uint32_t Offset(void) const { return m_Offset; }
    bool IsArray(void) const { return m_IsArray; }
    bool IsShadow(void) const { return m_IsShadow; }
    bool IsBuffer(void) const { return m_IsBuffer; }
    bool IsBindless(void) const { return m_IsBindless; }
    bool IsIndexed(void) const { return m_IsIndexed; }

private:
    ShaderSamplerEntry & operator=(const ShaderSamplerEntry & );

    ShaderTextureType m_Type;
    uint32_t m_Index;
    uint32_t m_Offset;
    uint32_t m_Size;
    bool m_IsArray;
    bool m_IsShadow;
    bool m_IsBuffer;
    bool m_IsBindless;
    bool m_IsIndexed;
};
typedef std::vector<ShaderSamplerEntry> ShaderSamplerEntryList;

class ShaderTexture 
{
public:
    ShaderTexture();
    ShaderTexture(ShaderSamplerEntry Sampler, ShaderNodePtr Array, ShaderNodePtr DepthCompare, ShaderNodeList AOffi, ShaderNodeList PTP, ShaderNodeList Derivates, ShaderNodePtr Bias, ShaderNodePtr Lod, ShaderNodePtr Component, uint32_t Element, ShaderNodePtr Index);
    ShaderTexture(const ShaderTexture&);

    const ShaderSamplerEntry & Sampler() const { return m_Sampler; };
    const ShaderNodePtr & Array() const { return m_Array; }
    const ShaderNodePtr & DepthCompare() const { return m_DepthCompare; }
    const ShaderNodeList & AOffi() const { return m_AOffi; }
    const ShaderNodeList & PTP() const { return m_PTP; }
    const ShaderNodePtr & Bias() const { return m_Bias; }
    const ShaderNodePtr & Index() const { return m_Index; }
    const uint32_t & Element() const { return m_Element; }

private:
    ShaderTexture& operator=(const ShaderTexture&);
    
    ShaderSamplerEntry m_Sampler;
    ShaderNodePtr m_Array;
    ShaderNodePtr m_DepthCompare;
    ShaderNodeList m_AOffi;
    ShaderNodeList m_Derivates;
    ShaderNodeList m_PTP;
    ShaderNodePtr m_Bias;
    ShaderNodePtr m_Lod;
    ShaderNodePtr m_Index;
    ShaderNodePtr m_Component;
    uint32_t m_Element;
};

enum ShaderOperationType 
{
    ShaderOperationType_Unknown,
    ShaderOperationType_Texture,
    ShaderOperationType_Operand,
};

class ShaderOperationNode : 
    public ShaderNode 
{
 public:
    ShaderNodeType GetShaderNodeType() const { return ShaderNodeType_Operation; }
    ShaderOperationNode * GetShaderOperationNode(void) { return this; }

    ShaderOperationNode(ShaderOperationCode Code);
    ShaderOperationNode(ShaderOperationCode Code, ShaderTexture MetaTexture, ShaderNodeList Operands);
    ShaderOperationNode(ShaderOperationCode Code, bool Precise, ShaderNodeList Operands);

    ShaderOperationCode GetCode() const { return m_Code; }
    size_t GetOperandsCount() const { return m_Operands.size(); }
    const ShaderNodePtr & GetOperand(std::size_t Index) const { return m_Operands[Index]; }
    bool IsPrecise() const { return m_Precise; }
    ShaderOperationType OperationType() const { return m_OperationType; }
    const ShaderTexture & GetTexture() const { return m_Texture; }

private:
    ShaderOperationNode();
    ShaderOperationNode(const ShaderOperationNode&);
    ShaderOperationNode & operator=(const ShaderOperationNode&);

    ShaderOperationType m_OperationType;
    ShaderOperationCode m_Code;
    ShaderTexture m_Texture;
    ShaderNodeList m_Operands;
    bool m_Precise;
};
