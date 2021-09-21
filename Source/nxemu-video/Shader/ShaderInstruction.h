#pragma once
#include <vector>
#include <set>
#include <stdint.h>

enum 
{ 
    ShaderRegister_ZeroIndex = 255
};

enum ShaderAttributeIndex
{
    ShaderAttributeIndex_LayerViewportPointSize = 6,
    ShaderAttributeIndex_Position = 7,
    ShaderAttributeIndex_Attribute_0 = 8,
    ShaderAttributeIndex_Attribute_31 = 39,
    ShaderAttributeIndex_FrontColor = 40,
    ShaderAttributeIndex_FrontSecondaryColor = 41,
    ShaderAttributeIndex_BackColor = 42,
    ShaderAttributeIndex_BackSecondaryColor = 43,
    ShaderAttributeIndex_ClipDistances0123 = 44,
    ShaderAttributeIndex_ClipDistances4567 = 45,
    ShaderAttributeIndex_PointCoord = 46,
    ShaderAttributeIndex_TessCoordInstanceIDVertexID = 47,
    ShaderAttributeIndex_TexCoord_0 = 48,
    ShaderAttributeIndex_TexCoord_7 = 55,
    ShaderAttributeIndex_FrontFacing = 63,
};

enum ShaderAttributeSize 
{
    ShaderAttributeSize_Word = 0,
    ShaderAttributeSize_DoubleWord = 1,
    ShaderAttributeSize_TripleWord = 2,
    ShaderAttributeSize_QuadWord = 3,
};

enum ShaderConditionCode : uint64_t 
{
    ShaderConditionCode_F = 0,
    ShaderConditionCode_LT = 1,
    ShaderConditionCode_EQ = 2,
    ShaderConditionCode_LE = 3,
    ShaderConditionCode_GT = 4,
    ShaderConditionCode_NE = 5,
    ShaderConditionCode_GE = 6,
    ShaderConditionCode_Num = 7,
    ShaderConditionCode_Nan = 8,
    ShaderConditionCode_LTU = 9,
    ShaderConditionCode_EQU = 10,
    ShaderConditionCode_LEU = 11,
    ShaderConditionCode_GTU = 12,
    ShaderConditionCode_NEU = 13,
    ShaderConditionCode_GEU = 14,
    ShaderConditionCode_T = 15,
    ShaderConditionCode_OFF = 16,
    ShaderConditionCode_LO = 17,
    ShaderConditionCode_SFF = 18,
    ShaderConditionCode_LS = 19,
    ShaderConditionCode_HI = 20,
    ShaderConditionCode_SFT = 21,
    ShaderConditionCode_HS = 22,
    ShaderConditionCode_OFT = 23,
    ShaderConditionCode_CSM_TA = 24,
    ShaderConditionCode_CSM_TR = 25,
    ShaderConditionCode_CSM_MX = 26,
    ShaderConditionCode_FCSM_TA = 27,
    ShaderConditionCode_FCSM_TR = 28,
    ShaderConditionCode_FCSM_MX = 29,
    ShaderConditionCode_RLE = 30,
    ShaderConditionCode_RGT = 31,
};

enum ShaderF2FRoundingOp : uint64_t
{
    ShaderF2FRoundingOp_None = 0,
    ShaderF2FRoundingOp_Pass = 3,
    ShaderF2FRoundingOp_Round = 8,
    ShaderF2FRoundingOp_Floor = 9,
    ShaderF2FRoundingOp_Ceil = 10,
    ShaderF2FRoundingOp_Trunc = 11,
};

enum ShaderIpaInterpMode : uint64_t 
{
    ShaderIpaInterpMode_Pass = 0,
    ShaderIpaInterpMode_Multiply = 1,
    ShaderIpaInterpMode_Constant = 2,
    ShaderIpaInterpMode_Sc = 3,
};

enum ShaderIpaSampleMode : uint64_t {
    ShaderIpaSampleMode_Default = 0,
    ShaderIpaSampleMode_Centroid = 1,
    ShaderIpaSampleMode_Offset = 2,
};

enum ShaderPredIndex : uint64_t
{
    ShaderPredIndex_Unused = 0x7,
    ShaderPredIndex_NeverExecute = 0xF,
};

enum ShaderRegisterSize : uint64_t
{
    ShaderRegisterSize_Byte = 0,
    ShaderRegisterSize_Short = 1,
    ShaderRegisterSize_Word = 2,
    ShaderRegisterSize_Long = 3,
};

enum ShaderSamplerIndex : unsigned
{
    ShaderSamplerIndex_0 = 8,
};

enum ShaderSubOp : uint64_t 
{
    ShaderSubOp_Cos = 0x0,
    ShaderSubOp_Sin = 0x1,
    ShaderSubOp_Ex2 = 0x2,
    ShaderSubOp_Lg2 = 0x3,
    ShaderSubOp_Rcp = 0x4,
    ShaderSubOp_Rsq = 0x5,
    ShaderSubOp_Sqrt = 0x8,
};

enum ShaderTextureMiscMode : uint64_t 
{
    ShaderTextureMiscMode_DC,
    ShaderTextureMiscMode_AOFFI,
    ShaderTextureMiscMode_NDV,
    ShaderTextureMiscMode_NODEP,
    ShaderTextureMiscMode_MZ,
    ShaderTextureMiscMode_PTP,
};

enum ShaderTextureProcessMode : uint64_t 
{
    ShaderTextureProcessMode_None = 0,
    ShaderTextureProcessMode_LZ = 1,
    ShaderTextureProcessMode_LB = 2,
    ShaderTextureProcessMode_LL = 3,
    ShaderTextureProcessMode_LBA = 6,
    ShaderTextureProcessMode_LLA = 7,
};

enum ShaderTextureType
{
    ShaderTextureType_1D = 0,
    ShaderTextureType_2D = 1,
    ShaderTextureType_3D = 2,
    ShaderTextureType_Cube = 3,
};

#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
union ShaderInstruction 
{
    typedef union _tyAlu
    {
        struct 
        {
            uint64_t : 20;
            uint64_t Imm20_19 : 19;
        };

        struct 
        {
            uint64_t : 45;
            uint64_t NegateB : 1;
            uint64_t AbsA : 1;
            uint64_t : 1;
            uint64_t NegateA : 1;
            uint64_t AbsB : 1;
            uint64_t SaturateD : 1;
            uint64_t : 5;
            uint64_t NegateImm : 1;
        };

        struct 
        {
            uint64_t : 39;
            uint64_t Pred : 3;
            uint64_t NegatePred : 1;
        } FMnMx;
    } tyAlu;

    typedef union _tyAttribute
    {
        union _Fmt20
        {
            struct 
            {
                unsigned : 20;
                unsigned Immediate : 10;
            };
            struct 
            {
                unsigned : 22;
                unsigned Element : 2;
                ShaderAttributeIndex Index : 6;
                unsigned : 1;
                unsigned Patch : 1;
                unsigned : 15;
                ShaderAttributeSize Size : 3;
            };
            bool IsPhysical() const;
        } Fmt20;

        struct 
        {
            unsigned : 30;
            unsigned Element : 2;
            ShaderAttributeIndex Index : 6;
        } Fmt28;
    } tyAttribute;

    typedef struct _tyCBuf34
    {
        uint64_t : 20;
        uint64_t ShiftedOffset : 14;
        uint64_t Index : 5;

        uint64_t GetOffset() const;
    } tyCBuf34;

    typedef union _tyConversion
    {
        struct 
        {
            uint64_t : 8;
            ShaderRegisterSize DstSize : 2;
            ShaderRegisterSize SrcSize : 2;
            uint64_t IsOutputSigned : 1;
            uint64_t IsInputSigned : 1;
            uint64_t : 31;
            uint64_t NegateA : 1;
            uint64_t : 3;
            uint64_t AbsA : 1;
        };

        struct 
        {
            uint64_t : 41;
            uint64_t Selector : 2;
        } IntSrc;

        struct
        {
            uint64_t : 41;
            uint64_t Selector : 1;
        } FloatSrc;

        union _F2F
        {
            struct 
            {
                uint64_t : 39;
                uint64_t Rounding : 4;
            };
            struct 
            {
                uint64_t : 41;
                uint64_t Selector : 1;
            };
            ShaderF2FRoundingOp GetRoundingMode() const;
        } F2F;
    } tyConversion;

    typedef struct 
    {
        uint64_t : 47;
        uint64_t CC : 1;
        uint64_t NegateB : 1;
        uint64_t NegateC : 1;
        uint64_t : 1;
        uint64_t Tab5980_1 : 2;
        uint64_t Tab5980_0 : 2;
    } tyFfma;

    typedef struct 
    {
        uint64_t : 39;
        uint64_t Tab5cb8_2 : 2;
        uint64_t PostFactor : 3;
        uint64_t Tab5c68_0 : 2;
        uint64_t : 2;
        uint64_t NegateB : 1;
    } tyFMul;

    typedef struct
    {
        uint64_t : 38;
        uint64_t Idx : 1;
        uint64_t : 12;
        uint64_t Saturate : 1;
        ShaderIpaSampleMode SampleMode : 2;
        ShaderIpaInterpMode InterpMode : 2;
    } tyIpa;

    typedef union
    {
        struct
        {
            uint64_t : 16;
            ShaderPredIndex Full : 4;
        };
        struct 
        {
            uint64_t : 16;
            uint64_t Index : 3;
        };
    } tyPred;

    typedef struct 
    {
        unsigned : 32;
        unsigned : 4;
        ShaderSamplerIndex Index : 13;
    } tySampler;

    typedef struct _tyTexs
    {
        uint64_t Gpr0 : 8;
        uint64_t : 20;
        uint64_t Gpr28 : 8;
        uint64_t : 13;
        uint64_t NodepFlag : 1;
        uint64_t ComponentMaskSelector : 3;
        uint64_t TextureInfo : 4;
        uint64_t : 2;
        uint64_t Fp32Flag : 1;

        bool IsArrayTexture() const;
        ShaderTextureProcessMode GetTextureProcessMode() const;
        ShaderTextureType GetTextureType() const;
        bool IsComponentEnabled(uint32_t Component) const; 
        bool HasTwoDestinations() const;
        bool UsesMiscMode(ShaderTextureMiscMode Mode) const;
    } tyTexs;

    uint64_t Value;
   
    struct 
    {
        uint64_t : 19;
        uint64_t NegatePred : 1;
        uint64_t : 12;
        uint64_t : 16;
        uint64_t Opcode : 16;
    };

    struct 
    {
        uint64_t Gpr0 : 8;
        uint64_t Gpr8 : 8;
        uint64_t : 4;
        uint64_t Gpr20 : 8;
        uint64_t Gpr28 : 8;
        uint64_t : 3;
        uint64_t Gpr39 : 8;
    };

    struct 
    {
        uint64_t : 47;
        uint64_t GeneratesCC : 1;
        uint64_t : 11;
        uint64_t IsCGpr : 1;
        uint64_t IsBGpr : 1;
        uint64_t IsBImm : 1;
    };

    struct
    {
        ShaderConditionCode FlowConditionCode : 5;
    };

    struct 
    {
        uint64_t : 20;
        ShaderSubOp SubOP : 4;
    };
    tyAlu Alu;
    tyAttribute Attribute;
    tyCBuf34 CBuf34;
    tyConversion Conversion;
    tyFfma Ffma;
    tyFMul FMul;
    tyIpa Ipa;
    tyPred Pred;
    tySampler Sampler;
    tyTexs Texs;
};
#pragma warning(pop)

typedef std::vector<ShaderInstruction> ShaderProgramCode;
typedef std::set<ShaderAttributeIndex> ShaderAttributeIndexSet;
