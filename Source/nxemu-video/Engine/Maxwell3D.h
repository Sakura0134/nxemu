#pragma once
#include "GpuTypes.h"
#include "Engine\UploadState.h"
#include "Macro\MacroEngine.h"
#include "Textures\Texture.h"
#include "Util\StateTracker.h"
#include "Util\Rectangle.h"
#include <Common\padding.h>
#include <stdint.h>
#include <cstddef>
#include <memory>

__interface ISwitchSystem;
__interface IRenderer;
class CVideoMemory;

class CMaxwell3D
{
public:
    enum
    {
        NumRegisters = 0xE00,
        NumViewPorts = 16,
        NumCBData = 16,
        NumVertexArrays = 32,
        MaxShaderStage = 5,
        MaxConstBuffers = 18,
        MaxConstBufferSize = 0x10000
    };

    enum BlendEquation : unsigned
    {
        BlendEquation_Add = 1,
        BlendEquation_Subtract = 2,
        BlendEquation_ReverseSubtract = 3,
        BlendEquation_Min = 4,
        BlendEquation_Max = 5,
        BlendEquation_AddGL = 0x8006,
        BlendEquation_SubtractGL = 0x8007,
        BlendEquation_ReverseSubtractGL = 0x8008,
        BlendEquation_MinGL = 0x800A,
        BlendEquation_MaxGL = 0x800B
    };

    enum BlendFactor : unsigned
    {
        BlendFactor_Zero = 0x1,
        BlendFactor_One = 0x2,
        BlendFactor_SourceColor = 0x3,
        BlendFactor_OneMinusSourceColor = 0x4,
        BlendFactor_SourceAlpha = 0x5,
        BlendFactor_OneMinusSourceAlpha = 0x6,
        BlendFactor_DestAlpha = 0x7,
        BlendFactor_OneMinusDestAlpha = 0x8,
        BlendFactor_DestColor = 0x9,
        BlendFactor_OneMinusDestColor = 0xa,
        BlendFactor_SourceAlphaSaturate = 0xb,
        BlendFactor_Source1Color = 0x10,
        BlendFactor_OneMinusSource1Color = 0x11,
        BlendFactor_Source1Alpha = 0x12,
        BlendFactor_OneMinusSource1Alpha = 0x13,
        BlendFactor_ConstantColor = 0x61,
        BlendFactor_OneMinusConstantColor = 0x62,
        BlendFactor_ConstantAlpha = 0x63,
        BlendFactor_OneMinusConstantAlpha = 0x64,
        BlendFactor_ZeroGL = 0x4000,
        BlendFactor_OneGL = 0x4001,
        BlendFactor_SourceColorGL = 0x4300,
        BlendFactor_OneMinusSourceColorGL = 0x4301,
        BlendFactor_SourceAlphaGL = 0x4302,
        BlendFactor_OneMinusSourceAlphaGL = 0x4303,
        BlendFactor_DestAlphaGL = 0x4304,
        BlendFactor_OneMinusDestAlphaGL = 0x4305,
        BlendFactor_DestColorGL = 0x4306,
        BlendFactor_OneMinusDestColorGL = 0x4307,
        BlendFactor_SourceAlphaSaturateGL = 0x4308,
        BlendFactor_ConstantColorGL = 0xc001,
        BlendFactor_OneMinusConstantColorGL = 0xc002,
        BlendFactor_ConstantAlphaGL = 0xc003,
        BlendFactor_OneMinusConstantAlphaGL = 0xc004,
        BlendFactor_Source1ColorGL = 0xc900,
        BlendFactor_OneMinusSource1ColorGL = 0xc901,
        BlendFactor_Source1AlphaGL = 0xc902,
        BlendFactor_OneMinusSource1AlphaGL = 0xc903,
    };

    enum ComparisonOp : unsigned
    {
        ComparisonOp_Never = 0x200,
        ComparisonOp_Less = 0x201,
        ComparisonOp_Equal = 0x202,
        ComparisonOp_LessEqual = 0x203,
        ComparisonOp_Greater = 0x204,
        ComparisonOp_NotEqual = 0x205,
        ComparisonOp_GreaterEqual = 0x206,
        ComparisonOp_Always = 0x207,

        ComparisonOp_NeverOld = 1,
        ComparisonOp_LessOld = 2,
        ComparisonOp_EqualOld = 3,
        ComparisonOp_LessEqualOld = 4,
        ComparisonOp_GreaterOld = 5,
        ComparisonOp_NotEqualOld = 6,
        ComparisonOp_GreaterEqualOld = 7,
        ComparisonOp_AlwaysOld = 8,
    };

    enum ConditionMode : unsigned
    {
        ConditionMode_Never = 0,
        ConditionMode_Always = 1,
        ConditionMode_ResNonZero = 2,
        ConditionMode_Equal = 3,
        ConditionMode_NotEqual = 4,
    };

    enum CounterReset : unsigned
    {
        CounterReset_SampleCnt = 0x01,
        CounterReset_Unk02 = 0x02,
        CounterReset_Unk03 = 0x03,
        CounterReset_Unk04 = 0x04,
        CounterReset_EmittedPrimitives = 0x10,
        CounterReset_Unk11 = 0x11,
        CounterReset_Unk12 = 0x12,
        CounterReset_Unk13 = 0x13,
        CounterReset_Unk15 = 0x15,
        CounterReset_Unk16 = 0x16,
        CounterReset_Unk17 = 0x17,
        CounterReset_Unk18 = 0x18,
        CounterReset_Unk1A = 0x1A,
        CounterReset_Unk1B = 0x1B,
        CounterReset_Unk1C = 0x1C,
        CounterReset_Unk1D = 0x1D,
        CounterReset_Unk1E = 0x1E,
        CounterReset_GeneratedPrimitives = 0x1F,
    };

    enum CullFace : unsigned
    {
        CullFace_Front = 0x0404,
        CullFace_Back = 0x0405,
        CullFace_FrontAndBack = 0x0408,
    };

    enum DepthMode : unsigned
    {
        DepthMode_MinusOneToOne = 0,
        DepthMode_ZeroToOne = 1,
    };

    enum FrontFace : unsigned
    {
        FrontFace_ClockWise = 0x0900,
        FrontFace_CounterClockWise = 0x0901,
    };

    enum IndexFormat : unsigned
    {
        IndexFormat_UnsignedByte = 0x0,
        IndexFormat_UnsignedShort = 0x1,
        IndexFormat_UnsignedInt = 0x2,
    };

    enum LogicOperation : unsigned
    {
        LogicOperation_Clear = 0x1500,
        LogicOperation_And = 0x1501,
        LogicOperation_AndReverse = 0x1502,
        LogicOperation_Copy = 0x1503,
        LogicOperation_AndInverted = 0x1504,
        LogicOperation_NoOp = 0x1505,
        LogicOperation_Xor = 0x1506,
        LogicOperation_Or = 0x1507,
        LogicOperation_Nor = 0x1508,
        LogicOperation_Equiv = 0x1509,
        LogicOperation_Invert = 0x150A,
        LogicOperation_OrReverse = 0x150B,
        LogicOperation_CopyInverted = 0x150C,
        LogicOperation_OrInverted = 0x150D,
        LogicOperation_Nand = 0x150E,
        LogicOperation_Set = 0x150F,
    };

    enum MMEDrawMode : unsigned
    {
        MMEDrawMode_Undefined,
        MMEDrawMode_Array,
        MMEDrawMode_Indexed,
    };

    enum PolygonMode : unsigned
    {
        PolygonMode_Point = 0x1b00,
        PolygonMode_Line = 0x1b01,
        PolygonMode_Fill = 0x1b02,
    };

    enum PrimitiveTopology : unsigned
    {
        PrimitiveTopology_Points = 0x0,
        PrimitiveTopology_Lines = 0x1,
        PrimitiveTopology_LineLoop = 0x2,
        PrimitiveTopology_LineStrip = 0x3,
        PrimitiveTopology_Triangles = 0x4,
        PrimitiveTopology_TriangleStrip = 0x5,
        PrimitiveTopology_TriangleFan = 0x6,
        PrimitiveTopology_Quads = 0x7,
        PrimitiveTopology_QuadStrip = 0x8,
        PrimitiveTopology_Polygon = 0x9,
        PrimitiveTopology_LinesAdjacency = 0xA,
        PrimitiveTopology_LineStripAdjacency = 0xB,
        PrimitiveTopology_TrianglesAdjacency = 0xC,
        PrimitiveTopology_TriangleStripAdjacency = 0xD,
        PrimitiveTopology_Patches = 0xE,
    };

    enum QueryOperation : unsigned
    {
        QueryOperation_Release = 0,
        QueryOperation_Acquire = 1,
        QueryOperation_Counter = 2,
        QueryOperation_Trap = 3,
    };

    enum QuerySelect : unsigned
    {
        QuerySelect_Zero = 0,
        QuerySelect_TimeElapsed = 2,
        QuerySelect_TransformFeedbackPrimitivesGenerated = 11,
        QuerySelect_PrimitivesGenerated = 18,
        QuerySelect_SamplesPassed = 21,
        QuerySelect_TransformFeedbackUnknown = 26,
    };

    enum QuerySyncCondition : unsigned
    {
        QuerySyncCondition_NotEqual = 0,
        QuerySyncCondition_GreaterThan = 1,
    };

    enum QueryUnit : unsigned
    {
        QueryUnit_VFetch = 1,
        QueryUnit_VP = 2,
        QueryUnit_Rast = 4,
        QueryUnit_StrmOut = 5,
        QueryUnit_GP = 6,
        QueryUnit_ZCull = 7,
        QueryUnit_Prop = 10,
        QueryUnit_Crop = 15,
    };

    enum ShadowRamControl : unsigned
    {
        ShadowRamControl_Track = 0,
        ShadowRamControl_TrackWithFilter = 1,
        ShadowRamControl_Passthrough = 2,
        ShadowRamControl_Replay = 3,
    };

    enum StencilOp : unsigned
    {
        StencilOp_Keep = 1,
        StencilOp_Zero = 2,
        StencilOp_Replace = 3,
        StencilOp_Incr = 4,
        StencilOp_Decr = 5,
        StencilOp_Invert = 6,
        StencilOp_IncrWrap = 7,
        StencilOp_DecrWrap = 8,
        StencilOp_KeepOGL = 0x1E00,
        StencilOp_ZeroOGL = 0,
        StencilOp_ReplaceOGL = 0x1E01,
        StencilOp_IncrOGL = 0x1E02,
        StencilOp_DecrOGL = 0x1E03,
        StencilOp_InvertOGL = 0x150A,
        StencilOp_IncrWrapOGL = 0x8507,
        StencilOp_DecrWrapOGL = 0x8508,
    };

    enum ViewportSwizzle : unsigned
    {
        ViewportSwizzle_PositiveX = 0,
        ViewportSwizzle_NegativeX = 1,
        ViewportSwizzle_PositiveY = 2,
        ViewportSwizzle_NegativeY = 3,
        ViewportSwizzle_PositiveZ = 4,
        ViewportSwizzle_NegativeZ = 5,
        ViewportSwizzle_PositiveW = 6,
        ViewportSwizzle_NegativeW = 7,
    };

#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
#pragma warning(disable : 5208) // warning C5208: unnamed class used in typedef name cannot declare members other than non-static
    typedef struct _tyAdrressLimit
    {
        uint32_t AddressHigh;
        uint32_t AddressLow;
        uint32_t Limit;
    } tyAdrressLimit;

    typedef struct
    {
        uint32_t AeparateAlpha;
        BlendEquation EquationRGB;
        BlendFactor FactorSourceRGB;
        BlendFactor FactorDestRGB;
        BlendEquation EquationA;
        BlendFactor FactorSourceA;
        PADDING_WORDS(1);
        BlendFactor FactorDestA;
        uint32_t EnableCommon;
        uint32_t Enable[NumRenderTargets];
    } tyBlend;

    typedef struct
    {
        float R;
        float G;
        float B;
        float A;
    } tyBlendColor;

    typedef struct
    {
        union
        {
            uint32_t Value;
            struct
            {
                unsigned Valid : 1;
                unsigned : 3;
                unsigned Index : 5;
            };
        };
        PADDING_WORDS(7);
    } tyCBBind;

    typedef union
    {
        uint32_t Value;
        struct
        {
            unsigned Z : 1;
            unsigned S : 1;
            unsigned R : 1;
            unsigned G : 1;
            unsigned B : 1;
            unsigned A : 1;
            unsigned RT : 4;
            unsigned Layer : 11;
        };
    } tyClearBuffers;

    typedef union
    {
        uint32_t Value;
        struct
        {
            unsigned Stencil : 4;
            unsigned : 4;
            unsigned Scissor : 4;
            unsigned Viewport : 4;
        };
    } tyClearFlags;

    typedef union
    {
        uint32_t Value;
        struct
        {
            unsigned R : 4;
            unsigned G : 4;
            unsigned B : 4;
            unsigned A : 4;
        };
    } tyColorMask;

    typedef struct _tyCondition
    {
        uint32_t AddressHigh;
        uint32_t AddressLow;
        ConditionMode Mode;

        uint64_t Address(void) const;
    } tyCondition;

    typedef struct _tyConstBuffer
    {
        uint32_t Size;
        uint32_t AddressHigh;
        uint32_t AddressLow;
        uint32_t Pos;
        uint32_t Data[NumCBData];

        uint64_t Address() const;
    } tyConstBuffer;

    typedef struct
    {
        uint32_t VertexEndGL;
        union
        {
            uint32_t VertexBeginGL;
            struct
            {
                PrimitiveTopology Topology : 16;
                unsigned : 10;
                unsigned InstanceNext : 1;
                unsigned InstanceCont : 1;
            };
        };
    } tyDraw;

    typedef union
    {
        uint32_t Value;
        struct
        {
            unsigned Linear : 1;
        };
    } tyExecUpload;

    typedef struct
    {
        uint32_t SeparateAlpha;
        BlendEquation EquationRGB;
        BlendFactor FactorSourceRGB;
        BlendFactor FactorDestRGB;
        BlendEquation EquationA;
        BlendFactor FactorSourceA;
        BlendFactor FactorDestA;
        PADDING_WORDS(1);
    } tyIndependentBlend;

    typedef struct _tyIndexArray
    {
        uint32_t StartAddrHigh;
        uint32_t StartAddrLow;
        uint32_t EndAddrHigh;
        uint32_t EndAddrLow;
        IndexFormat Format;
        uint32_t First;
        uint32_t Count;

        unsigned FormatSizeInBytes() const;
        uint64_t StartAddress() const;
    } tyIndexArray;

    typedef struct
    {
        uint32_t Enable;
        LogicOperation Operation;
    } tyLogicOp;

    typedef struct
    {
        uint32_t UploadAddress;
        uint32_t Data;
        uint32_t Entry;
        uint32_t Bind;
    } tyMacros;

    typedef struct
    {
        MMEDrawMode CurrentMode;
        uint32_t CurrentCount;
        uint32_t InstanceCount;
        bool InstanceMode;
        bool BeginConsume;
        uint32_t EndCount;
    } tyMMEDrawState;

    typedef union
    {
        uint32_t Value;
        struct
        {
            unsigned AlphaToCoverage : 1;
            unsigned : 3;
            unsigned AlphaToOne : 1;
        };
    } tyMultiSampleControl;

    typedef union
    {
        uint32_t Value;
        struct
        {
            unsigned Enable : 1;
        };
    } tyVPPointSize;

    typedef struct
    {
        uint32_t Enabled;
        uint32_t Index;
    } tyPrimitiveRestart;

    typedef struct _tyQuery
    {
        uint32_t AddressHigh;
        uint32_t AddressLow;
        uint32_t QuerySequence;
        union
        {
            uint32_t Value;
            struct
            {
                QueryOperation Operation : 2;
                unsigned : 2;
                unsigned Fence : 1;
                unsigned : 7;
                QueryUnit Unit : 4;
                QuerySyncCondition SyncCond : 1;
                unsigned : 6;
                QuerySelect Select : 5;
                unsigned ShortQuery : 1;
            };
        } QueryGet;

        uint64_t Address() const;
    } tyQuery;

    typedef struct
    {
        unsigned X : 16;
        unsigned Width : 16;
        unsigned Y : 16;
        unsigned Height : 16;
    } tyRenderArea;

    typedef union
    {
        uint32_t Value;
        struct
        {
            unsigned BlockWidth : 4;
            unsigned BlockHeight : 4;
            unsigned BlockDepth : 4;
            unsigned IsPitchLinear : 1;
            unsigned : 3;
            unsigned Is3D : 1;
        };
    } tyTileMode;

    typedef struct _tyRenderTarget
    {
        uint32_t AddressHigh;
        uint32_t AddressLow;
        uint32_t Width;
        uint32_t Height;
        RenderTargetFormat Format;
        tyTileMode TileMode;
        union
        {
            uint32_t DepthRaw;
            struct
            {
                unsigned Depth : 16;
                unsigned Volume : 1;
            };
        };
        uint32_t LayerStride;
        uint32_t BaseLayer;
        PADDING_WORDS(7);

        uint64_t Address() const;
    } tyRenderTarget;

    typedef union _tyRTControl
    {
        uint32_t Value;
        struct
        {
            unsigned Count : 4;
            unsigned Map0 : 3;
            unsigned Map1 : 3;
            unsigned Map2 : 3;
            unsigned Map3 : 3;
            unsigned Map4 : 3;
            unsigned Map5 : 3;
            unsigned Map6 : 3;
            unsigned Map7 : 3;
        };
        uint32_t Map(uint32_t Index) const;
    } tyRTControl;

    typedef struct
    {
        uint32_t Enable;
        unsigned MinX : 16;
        unsigned MaxX : 16;
        unsigned MinY : 16;
        unsigned MaxY : 16;
        uint32_t Fill;
    } tyScissorTest;

    typedef union
    {
        uint32_t Value;
        struct {
            unsigned YNegate : 1;
            unsigned : 3;
            unsigned TriangleRastFlip : 1;
        };
    } tyScreenYControl;

    typedef struct
    {
        uint64_t Address;
        uint32_t Size;
        bool Enabled;
    } tyShaderStage;

    typedef union
    {
        uint32_t Value;
        struct
        {
            unsigned SyncPoint : 16;
            unsigned : 4;
            unsigned Increment : 1;
        };
    } tySyncInfo;
    typedef struct _tyVertexArray
    {
        union
        {
            uint32_t StrideValue;
            struct
            {
                unsigned Stride : 12;
                unsigned Enable : 1;
            };
        };
        uint32_t StartHigh;
        uint32_t StartLow;
        uint32_t Divisor;

        uint64_t StartAddress() const;
        bool IsEnabled() const;
    } tyVertexArray;

    typedef struct _tyVertexArrayLimit
    {
        uint32_t LimitHigh;
        uint32_t LimitLow;

        uint64_t LimitAddress() const;
    } tyVertexArrayLimit;


    typedef struct
    {
        unsigned X : 16;
        unsigned Width : 16;
        unsigned Y : 16;
        unsigned Height : 16;
        float DepthRangeNear;
        float DepthRangeFar;
    } tyViewPort;

    typedef struct _tyViewPortTransform
    {
        float ScaleX;
        float ScaleY;
        float ScaleZ;
        float TranslateX;
        float TranslateY;
        float TranslateZ;
        union
        {
            uint32_t Value;
            struct
            {
                ViewportSwizzle X : 3;
                unsigned : 1;
                ViewportSwizzle Y : 3;
                unsigned : 1;
                ViewportSwizzle Z : 3;
                unsigned : 1;
                ViewportSwizzle W : 3;
            };
        } Swizzle;
        PADDING_WORDS(1);

        CRectangle<float> GetRect() const;
        float GetX() const;
        float GetY() const;
        float GetWidth() const;
        float GetHeight() const;
    } tyViewPortTransform;

    typedef union
    {
        uint32_t Value;
        struct
        {
            unsigned DepthRange : 1;
            unsigned : 2;
            unsigned DepthClampNear : 1;
            unsigned DepthClampFar : 1;
            unsigned : 6;
            unsigned DepthClampDisabled : 1;
        };
    } tyViewVolumeClipControl;

    typedef struct _tyZeta
    {
        uint32_t AddressHigh;
        uint32_t AddressLow;
        DepthFormat Format;
        tyTileMode TileMode;
        uint32_t LayerStride;

        uint64_t Address() const;
    } tyZeta;

    union Registers
    {
        struct
        {
            PADDING_WORDS(0x44);
            uint32_t WaitForIdle;
            tyMacros Macros;
            ShadowRamControl ShadowRamControl;
            PADDING_WORDS(0x16);
            CUploadState::Registers Upload;
            tyExecUpload ExecUpload;
            uint32_t DataUpload;
            PADDING_WORDS(0x44);
            tySyncInfo SyncInfo;
            PADDING_WORDS(0x2C);
            uint32_t RasterizeEnable;
            PADDING_WORDS(0x120);
            tyRenderTarget RenderTarget[NumRenderTargets];
            tyViewPortTransform ViewPortTransform[NumViewPorts];
            tyViewPort ViewPorts[NumViewPorts];
            PADDING_WORDS(0x1E);
            uint32_t VertexBufferCount;
            DepthMode DepthMode;
            float ClearColor[4];
            float ClearDepth;
            PADDING_WORDS(0x3);
            int32_t ClearStencil;
            PADDING_WORDS(0x2);
            PolygonMode PolygonModeFront;
            PolygonMode PolygonModeBack;
            PADDING_WORDS(0x3);
            uint32_t PolygonOffsetPointEnable;
            uint32_t PolygonOffsetLineEnable;
            uint32_t PolygonOffsetFillEnable;
            PADDING_WORDS(0x5);
            uint32_t FragmentBarrier;
            PADDING_WORDS(0x7);
            tyScissorTest ScissorTest[NumViewPorts];
            PADDING_WORDS(0x15);
            int32_t StencilBackFuncRef;
            uint32_t StencilBackMask;
            uint32_t StencilBackFuncMask;
            PADDING_WORDS(0x7);
            uint32_t TiledCacheBarrier;
            PADDING_WORDS(0x4);
            uint32_t ColorMaskCommon;
            PADDING_WORDS(0x13);
            tyZeta Zeta;
            tyRenderArea RenderArea;
            PADDING_WORDS(0x3F);
            tyClearFlags ClearFlags;
            PADDING_WORDS(0x10);
            uint32_t FillRectangle;
            PADDING_WORDS(0x37);
            tyRTControl RTControl;
            PADDING_WORDS(0x2);
            uint32_t ZetaWidth;
            uint32_t ZetaHeight;
            PADDING_WORDS(0x27);
            uint32_t DepthTestEnable;
            PADDING_WORDS(0x5);
            uint32_t IndependentBlendEnable;
            uint32_t DepthWriteEnabled;
            uint32_t AlphaTestEnabled;
            PADDING_WORDS(0x7);
            ComparisonOp DepthTestFunc;
            float AlphaTestRef;
            ComparisonOp AlphaTestFunc;
            PADDING_WORDS(0x1);
            tyBlendColor BlendColor;
            PADDING_WORDS(0x4);
            tyBlend Blend;
            uint32_t StencilEnable;
            StencilOp StencilFrontOpFail;
            StencilOp StencilFrontOpZFail;
            StencilOp StencilFrontOpZPass;
            ComparisonOp StencilFrontFuncFunc;
            int32_t StencilFrontFuncRef;
            uint32_t StencilFrontFuncMask;
            uint32_t StencilFrontMask;
            PADDING_WORDS(0x2);
            uint32_t FragmentColorClamp;
            tyScreenYControl ScreenYControl;
            float LineWidthSmooth;
            float LineWidthAliased;
            PADDING_WORDS(0x58);
            float PointSize;
            PADDING_WORDS(0x1);
            uint32_t PointSpriteEnable;
            PADDING_WORDS(0x3);
            CounterReset CounterReset;
            PADDING_WORDS(0x1);
            uint32_t ZetaEnable;
            tyMultiSampleControl MultisampleControl;
            PADDING_WORDS(0x4);
            tyCondition Condition;
            tyAdrressLimit Tsc;
            PADDING_WORDS(0x1);
            float PolygonOffsetFactor;
            uint32_t LineSmoothEnable;
            tyAdrressLimit Tic;
            PADDING_WORDS(0x5);
            uint32_t StencilTwoSideEnable;
            StencilOp StencilBackOpFail;
            StencilOp StencilBackOpZFail;
            StencilOp StencilBackOpZPass;
            ComparisonOp StencilBackFuncFunc;
            PADDING_WORDS(0x4);
            uint32_t FramebufferSRGB;
            float PolygonOffsetUnits;
            PADDING_WORDS(0x4);
            MsaaMode MultisampleMode;
            PADDING_WORDS(0x10);
            tyDraw Draw;
            PADDING_WORDS(0xA);
            tyPrimitiveRestart PrimitiveRestart;
            PADDING_WORDS(0x5F);
            tyIndexArray IndexArray;
            PADDING_WORDS(0x26);
            float PolygonOffsetClamp;
            PADDING_WORDS(0x24);
            tyVPPointSize VPPointSize;
            PADDING_WORDS(1);
            uint32_t CullTestEnabled;
            FrontFace FrontFace;
            CullFace CullFace;
            PADDING_WORDS(0x2);
            uint32_t ViewportTransformEnabled;
            PADDING_WORDS(0x3);
            tyViewVolumeClipControl ViewVolumeClipControl;
            PADDING_WORDS(0x21);
            tyLogicOp LogicOp;
            PADDING_WORDS(0x1);
            tyClearBuffers ClearBuffers;
            PADDING_WORDS(0xB);
            tyColorMask ColorMask[NumRenderTargets];
            PADDING_WORDS(0x38);
            tyQuery Query;
            PADDING_WORDS(0x3C);
            tyVertexArray VertexArray[NumVertexArrays];
            tyIndependentBlend IndependentBlend[NumRenderTargets];
            tyVertexArrayLimit VertexArrayLimit[NumVertexArrays];
            PADDING_WORDS(0xC0);
            uint32_t Firmware[0x20];
            tyConstBuffer ConstBuffer;
            PADDING_WORDS(0x10);
            tyCBBind CBBind[MaxShaderStage];
        };
        uint32_t Value[NumRegisters];
    };
#pragma warning(pop)

    enum Method : uint32_t
    {
        Method_AlphaTestEnabled = offsetof(Registers, AlphaTestEnabled) / sizeof(uint32_t),
        Method_AlphaTestFunc = offsetof(Registers, AlphaTestFunc) / sizeof(uint32_t),
        Method_AlphaTestRef = offsetof(Registers, AlphaTestRef) / sizeof(uint32_t),
        Method_Blend = offsetof(Registers, Blend) / sizeof(uint32_t),
        Method_BlendColor = offsetof(Registers, BlendColor) / sizeof(uint32_t),
        Method_BlendEnable = offsetof(Registers, Blend.Enable) / sizeof(uint32_t),
        Method_CBBind0 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 0)) / sizeof(uint32_t),
        Method_CBBind1 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 1)) / sizeof(uint32_t),
        Method_CBBind2 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 2)) / sizeof(uint32_t),
        Method_CBBind3 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 3)) / sizeof(uint32_t),
        Method_CBBind4 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 4)) / sizeof(uint32_t),
        Method_ClearBuffers = offsetof(Registers, ClearBuffers) / sizeof(uint32_t),
        Method_ClearFlags = offsetof(Registers, ClearFlags) / sizeof(uint32_t),
        Method_ColorMask = offsetof(Registers, ColorMask) / sizeof(uint32_t),
        Method_ColorMaskCommon = offsetof(Registers, ColorMaskCommon) / sizeof(uint32_t),
        Method_ConditionMode = offsetof(Registers, Condition.Mode) / sizeof(uint32_t),
        Method_ConstBufferData0 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 0)) / sizeof(uint32_t),
        Method_ConstBufferData1 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 1)) / sizeof(uint32_t),
        Method_ConstBufferData2 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 2)) / sizeof(uint32_t),
        Method_ConstBufferData3 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 3)) / sizeof(uint32_t),
        Method_ConstBufferData4 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 4)) / sizeof(uint32_t),
        Method_ConstBufferData5 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 5)) / sizeof(uint32_t),
        Method_ConstBufferData6 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 6)) / sizeof(uint32_t),
        Method_ConstBufferData7 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 7)) / sizeof(uint32_t),
        Method_ConstBufferData8 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 8)) / sizeof(uint32_t),
        Method_ConstBufferData9 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 9)) / sizeof(uint32_t),
        Method_ConstBufferData10 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 10)) / sizeof(uint32_t),
        Method_ConstBufferData11 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 11)) / sizeof(uint32_t),
        Method_ConstBufferData12 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 12)) / sizeof(uint32_t),
        Method_ConstBufferData13 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 13)) / sizeof(uint32_t),
        Method_ConstBufferData14 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 14)) / sizeof(uint32_t),
        Method_ConstBufferData15 = (offsetof(Registers, ConstBuffer.Data) + (sizeof(Registers::ConstBuffer.Data[0]) * 15)) / sizeof(uint32_t),
        Method_CounterReset = offsetof(Registers, CounterReset) / sizeof(uint32_t),
        Method_CullFace = offsetof(Registers, CullFace) / sizeof(uint32_t),
        Method_CullTestEnabled = offsetof(Registers, CullTestEnabled) / sizeof(uint32_t),
        Method_DataUpload = offsetof(Registers, DataUpload) / sizeof(uint32_t),
        Method_DepthMode = offsetof(Registers, DepthMode) / sizeof(uint32_t),
        Method_DepthTestEnable = offsetof(Registers, DepthTestEnable) / sizeof(uint32_t),
        Method_DepthTestFunc = offsetof(Registers, DepthTestFunc) / sizeof(uint32_t),
        Method_DepthWriteEnabled = offsetof(Registers, DepthWriteEnabled) / sizeof(uint32_t),
        Method_DrawVertexBeginGL = offsetof(Registers, Draw.VertexBeginGL) / sizeof(uint32_t),
        Method_DrawVertexEndGL = offsetof(Registers, Draw.VertexEndGL) / sizeof(uint32_t),
        Method_ExecUpload = offsetof(Registers, ExecUpload) / sizeof(uint32_t),
        Method_FillRectangle = offsetof(Registers, FillRectangle) / sizeof(uint32_t),
        Method_Firmware4 = (offsetof(Registers, Firmware) + (sizeof(Registers::Firmware[0]) * 4)) / sizeof(uint32_t),
        Method_FragmentBarrier = offsetof(Registers, FragmentBarrier) / sizeof(uint32_t),
        Method_FragmentColorClamp = offsetof(Registers, FragmentColorClamp) / sizeof(uint32_t),
        Method_FramebufferSRGB = offsetof(Registers, FramebufferSRGB) / sizeof(uint32_t),
        Method_FrontFace = offsetof(Registers, FrontFace) / sizeof(uint32_t),
        Method_IndependentBlend = offsetof(Registers, IndependentBlend) / sizeof(uint32_t),
        Method_IndependentBlendEnable = offsetof(Registers, IndependentBlendEnable) / sizeof(uint32_t),
        Method_IndexArrayCount = offsetof(Registers, IndexArray.Count) / sizeof(uint32_t),
        Method_LineSmoothEnable = offsetof(Registers, LineSmoothEnable) / sizeof(uint32_t),
        Method_LineWidthAliased = offsetof(Registers, LineWidthAliased) / sizeof(uint32_t),
        Method_LineWidthSmooth = offsetof(Registers, LineWidthSmooth) / sizeof(uint32_t),
        Method_LogicOp = offsetof(Registers, LogicOp) / sizeof(uint32_t),
        Method_MacrosBind = offsetof(Registers, Macros.Bind) / sizeof(uint32_t),
        Method_MacrosData = offsetof(Registers, Macros.Data) / sizeof(uint32_t),
        Method_MultisampleControl = offsetof(Registers, MultisampleControl) / sizeof(uint32_t),
        Method_PointSize = offsetof(Registers, PointSize) / sizeof(uint32_t),
        Method_PointSpriteEnable = offsetof(Registers, PointSpriteEnable) / sizeof(uint32_t),
        Method_PolygonModeBack = offsetof(Registers, PolygonModeBack) / sizeof(uint32_t),
        Method_PolygonModeFront = offsetof(Registers, PolygonModeFront) / sizeof(uint32_t),
        Method_PolygonOffsetClamp = offsetof(Registers, PolygonOffsetClamp) / sizeof(uint32_t),
        Method_PolygonOffsetFactor = offsetof(Registers, PolygonOffsetFactor) / sizeof(uint32_t),
        Method_PolygonOffsetFillEnable = offsetof(Registers, PolygonOffsetFillEnable) / sizeof(uint32_t),
        Method_PolygonOffsetLineEnable = offsetof(Registers, PolygonOffsetLineEnable) / sizeof(uint32_t),
        Method_PolygonOffsetPointEnable = offsetof(Registers, PolygonOffsetPointEnable) / sizeof(uint32_t),
        Method_PolygonOffsetUnits = offsetof(Registers, PolygonOffsetUnits) / sizeof(uint32_t),
        Method_PrimitiveRestart = offsetof(Registers, PrimitiveRestart) / sizeof(uint32_t),
        Method_QueryGet = offsetof(Registers, Query.QueryGet) / sizeof(uint32_t),
        Method_RasterizeEnable = offsetof(Registers, RasterizeEnable) / sizeof(uint32_t),
        Method_RenderArea = offsetof(Registers, RenderArea) / sizeof(uint32_t),
        Method_RenderTarget = offsetof(Registers, RenderTarget) / sizeof(uint32_t),
        Method_RTControl = offsetof(Registers, RTControl) / sizeof(uint32_t),
        Method_ScissorTest = offsetof(Registers, ScissorTest) / sizeof(uint32_t),
        Method_ScreenYControl = offsetof(Registers, ScreenYControl) / sizeof(uint32_t),
        Method_ShadowRamControl = offsetof(Registers, ShadowRamControl) / sizeof(uint32_t),
        Method_StencilBackFuncFunc = offsetof(Registers, StencilBackFuncFunc) / sizeof(uint32_t),
        Method_StencilBackFuncMask = offsetof(Registers, StencilBackFuncMask) / sizeof(uint32_t),
        Method_StencilBackFuncRef = offsetof(Registers, StencilBackFuncRef) / sizeof(uint32_t),
        Method_StencilBackMask = offsetof(Registers, StencilBackMask) / sizeof(uint32_t),
        Method_StencilBackOpFail = offsetof(Registers, StencilBackOpFail) / sizeof(uint32_t),
        Method_StencilBackOpZFail = offsetof(Registers, StencilBackOpZFail) / sizeof(uint32_t),
        Method_StencilBackOpZPass = offsetof(Registers, StencilBackOpZPass) / sizeof(uint32_t),
        Method_StencilEnable = offsetof(Registers, StencilEnable) / sizeof(uint32_t),
        Method_StencilFrontFuncFunc = offsetof(Registers, StencilFrontFuncFunc) / sizeof(uint32_t),
        Method_StencilFrontFuncMask = offsetof(Registers, StencilFrontFuncMask) / sizeof(uint32_t),
        Method_StencilFrontFuncRef = offsetof(Registers, StencilFrontFuncRef) / sizeof(uint32_t),
        Method_StencilFrontMask = offsetof(Registers, StencilFrontMask) / sizeof(uint32_t),
        Method_StencilFrontOpFail = offsetof(Registers, StencilFrontOpFail) / sizeof(uint32_t),
        Method_StencilFrontOpZFail = offsetof(Registers, StencilFrontOpZFail) / sizeof(uint32_t),
        Method_StencilFrontOpZPass = offsetof(Registers, StencilFrontOpZPass) / sizeof(uint32_t),
        Method_StencilTwoSideEnable = offsetof(Registers, StencilTwoSideEnable) / sizeof(uint32_t),
        Method_SyncInfo = offsetof(Registers, SyncInfo) / sizeof(uint32_t),
        Method_Tic = offsetof(Registers, Tic) / sizeof(uint32_t),
        Method_TiledCacheBarrier = offsetof(Registers, TiledCacheBarrier) / sizeof(uint32_t),
        Method_Tsc = offsetof(Registers, Tsc) / sizeof(uint32_t),
        Method_VertexArray = offsetof(Registers, VertexArray) / sizeof(uint32_t),
        Method_VertexArrayLimit = offsetof(Registers, VertexArrayLimit) / sizeof(uint32_t),
        Method_VertexBufferCount = offsetof(Registers, VertexBufferCount) / sizeof(uint32_t),
        Method_ViewPorts = offsetof(Registers, ViewPorts) / sizeof(uint32_t),
        Method_ViewPortTransform = offsetof(Registers, ViewPortTransform) / sizeof(uint32_t),
        Method_ViewportTransformEnabled = offsetof(Registers, ViewportTransformEnabled) / sizeof(uint32_t),
        Method_ViewVolumeClipControl = offsetof(Registers, ViewVolumeClipControl) / sizeof(uint32_t),
        Method_VPPointSize = offsetof(Registers, VPPointSize) / sizeof(uint32_t),
        Method_WaitForIdle = offsetof(Registers, WaitForIdle) / sizeof(uint32_t),
        Method_Zeta = offsetof(Registers, Zeta) / sizeof(uint32_t),
        Method_ZetaEnable = offsetof(Registers, ZetaEnable) / sizeof(uint32_t),
        Method_ZetaHeight = offsetof(Registers, ZetaHeight) / sizeof(uint32_t),
        Method_ZetaWidth = offsetof(Registers, ZetaWidth) / sizeof(uint32_t),
    };

    CMaxwell3D(ISwitchSystem & SwitchSystem, CVideoMemory & VideoMemory);
    ~CMaxwell3D();

    void BindRenderer(IRenderer * Renderer);
    void CallMultiMethod(Method Method, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending);
    void CallMethodFromMME(Method Method, uint32_t Argument);
    void FlushMMEInlineDraw();
    void CallMethod(Method Method, uint32_t Argument, bool Last);

    inline const Registers & Regs (void) const { return m_Regs; }
    inline CStateTracker & StateTracker (void) { return m_StateTracker; }

private:
    CMaxwell3D();
    CMaxwell3D(const CMaxwell3D&);
    CMaxwell3D& operator=(const CMaxwell3D&);

    struct CBDataState
    {
        uint32_t Buffer[0x4000][16];
        uint32_t Current;
        uint32_t Id;
        uint32_t StartPos;
        uint32_t Counter;
    };

    void InitializeRegisterDefaults();
    void ProcessMacro(uint32_t Method, const uint32_t * BaseStart, uint32_t Amount, bool Last);
    uint32_t ProcessShadowRam(uint32_t Method, uint32_t Argument);
    void ProcessMethodCall(Method Method, uint32_t ShadowArgument, uint32_t Argument, bool Last);
    void CallMacroMethod(uint32_t Method, const MacroParams & Parameters);
    void ProcessMacroBind(uint32_t data);
    void ProcessFirmwareCall4();
    void ProcessClearBuffers();
    void ProcessQueryGet();
    void ProcessSyncPoint();
    void StartCBData(uint32_t Method);
    void ProcessCBData(uint32_t Value);
    void ProcessCBMultiData(uint32_t Method, const uint32_t * BaseStart, uint32_t Amount);
    void FinishCBData();
    void ProcessCBBind(uint32_t StageIndex);
    void StepInstance(MMEDrawMode ExpectedMode, uint32_t Count);

    ISwitchSystem & m_SwitchSystem;
    CVideoMemory & m_VideoMemory;
    IRenderer * m_Renderer;
    Registers m_Regs, m_ShadowRegs;
    uint32_t m_MacroPositions[0x80];
    std::unique_ptr<MacroEngine> m_MacroEngine;
    CBDataState m_CBDataState;
    tyMMEDrawState m_MMEDraw;
    uint32_t m_ExecutingMacro;
    MacroParams m_MacroParams;
    CUploadState m_UploadState;
    CStateTracker m_StateTracker;
    tyShaderStage m_ShaderStage[MaxShaderStage][MaxConstBuffers];

    static_assert(sizeof(Registers) == NumRegisters * sizeof(uint32_t), "Maxwell3D Registers has wrong size");
};

#define ASSERT_REG_POSITION(field_name, position) static_assert(offsetof(CMaxwell3D::Registers, field_name) == position * 4, "Field " #field_name " has invalid position")

ASSERT_REG_POSITION(WaitForIdle, 0x44);
ASSERT_REG_POSITION(Macros, 0x45);
ASSERT_REG_POSITION(ShadowRamControl, 0x49);
ASSERT_REG_POSITION(Upload, 0x60);
ASSERT_REG_POSITION(ExecUpload, 0x6C);
ASSERT_REG_POSITION(DataUpload, 0x6D);
ASSERT_REG_POSITION(SyncInfo, 0xB2);
ASSERT_REG_POSITION(RasterizeEnable, 0xDF);
ASSERT_REG_POSITION(RenderTarget, 0x200);
ASSERT_REG_POSITION(ViewPorts, 0x300);
ASSERT_REG_POSITION(VertexBufferCount, 0x35E);
ASSERT_REG_POSITION(DepthMode, 0x35F);
ASSERT_REG_POSITION(ClearColor, 0x360);
ASSERT_REG_POSITION(ClearDepth, 0x364);
ASSERT_REG_POSITION(ClearStencil, 0x368);
ASSERT_REG_POSITION(PolygonModeFront, 0x36B);
ASSERT_REG_POSITION(PolygonModeBack, 0x36C);
ASSERT_REG_POSITION(PolygonOffsetPointEnable, 0x370);
ASSERT_REG_POSITION(PolygonOffsetLineEnable, 0x371);
ASSERT_REG_POSITION(PolygonOffsetFillEnable, 0x372);
ASSERT_REG_POSITION(FragmentBarrier, 0x378);
ASSERT_REG_POSITION(ScissorTest, 0x380);
ASSERT_REG_POSITION(StencilBackFuncRef, 0x3D5);
ASSERT_REG_POSITION(StencilBackMask, 0x3D6);
ASSERT_REG_POSITION(StencilBackFuncMask, 0x3D7);
ASSERT_REG_POSITION(TiledCacheBarrier, 0x3DF);
ASSERT_REG_POSITION(ColorMaskCommon, 0x3E4);
ASSERT_REG_POSITION(Zeta, 0x3F8);
ASSERT_REG_POSITION(RenderArea, 0x3FD);
ASSERT_REG_POSITION(ClearFlags, 0x43E);
ASSERT_REG_POSITION(FillRectangle, 0x44F);
ASSERT_REG_POSITION(RTControl, 0x487);
ASSERT_REG_POSITION(ZetaWidth, 0x48a);
ASSERT_REG_POSITION(ZetaHeight, 0x48b);
ASSERT_REG_POSITION(DepthTestEnable, 0x4B3);
ASSERT_REG_POSITION(IndependentBlendEnable, 0x4B9);
ASSERT_REG_POSITION(DepthWriteEnabled, 0x4BA);
ASSERT_REG_POSITION(AlphaTestEnabled, 0x4BB);
ASSERT_REG_POSITION(DepthTestFunc, 0x4C3);
ASSERT_REG_POSITION(AlphaTestRef, 0x4C4);
ASSERT_REG_POSITION(AlphaTestFunc, 0x4C5);
ASSERT_REG_POSITION(BlendColor, 0x4C7);
ASSERT_REG_POSITION(Blend, 0x4CF);
ASSERT_REG_POSITION(StencilEnable, 0x4E0);
ASSERT_REG_POSITION(StencilFrontOpFail, 0x4E1);
ASSERT_REG_POSITION(StencilFrontOpZFail, 0x4E2);
ASSERT_REG_POSITION(StencilFrontOpZPass, 0x4E3);
ASSERT_REG_POSITION(StencilFrontFuncFunc, 0x4E4);
ASSERT_REG_POSITION(StencilFrontFuncRef, 0x4E5);
ASSERT_REG_POSITION(StencilFrontFuncMask, 0x4E6);
ASSERT_REG_POSITION(StencilFrontMask, 0x4E7);
ASSERT_REG_POSITION(FragmentColorClamp, 0x4EA);
ASSERT_REG_POSITION(ScreenYControl, 0x4EB);
ASSERT_REG_POSITION(LineWidthSmooth, 0x4EC);
ASSERT_REG_POSITION(LineWidthAliased, 0x4ED);
ASSERT_REG_POSITION(PointSize, 0x546);
ASSERT_REG_POSITION(PointSpriteEnable, 0x548);
ASSERT_REG_POSITION(CounterReset, 0x54C);
ASSERT_REG_POSITION(ZetaEnable, 0x54E);
ASSERT_REG_POSITION(MultisampleControl, 0x54F);
ASSERT_REG_POSITION(Condition, 0x554);
ASSERT_REG_POSITION(Tsc, 0x557);
ASSERT_REG_POSITION(PolygonOffsetFactor, 0x55B);
ASSERT_REG_POSITION(LineSmoothEnable, 0x55C);
ASSERT_REG_POSITION(Tic, 0x55D);
ASSERT_REG_POSITION(StencilTwoSideEnable, 0x565);
ASSERT_REG_POSITION(StencilBackOpFail, 0x566);
ASSERT_REG_POSITION(StencilBackOpZFail, 0x567);
ASSERT_REG_POSITION(StencilBackOpZPass, 0x568);
ASSERT_REG_POSITION(StencilBackFuncFunc, 0x569);
ASSERT_REG_POSITION(FramebufferSRGB, 0x56E);
ASSERT_REG_POSITION(PolygonOffsetUnits, 0x56F);
ASSERT_REG_POSITION(MultisampleMode, 0x574);
ASSERT_REG_POSITION(Draw, 0x585);
ASSERT_REG_POSITION(PrimitiveRestart, 0x591);
ASSERT_REG_POSITION(IndexArray, 0x5F2);
ASSERT_REG_POSITION(PolygonOffsetClamp, 0x61F);
ASSERT_REG_POSITION(VPPointSize, 0x644);
ASSERT_REG_POSITION(CullTestEnabled, 0x646);
ASSERT_REG_POSITION(FrontFace, 0x647);
ASSERT_REG_POSITION(CullFace, 0x648);
ASSERT_REG_POSITION(ViewportTransformEnabled, 0x64B);
ASSERT_REG_POSITION(ViewVolumeClipControl, 0x64F);
ASSERT_REG_POSITION(LogicOp, 0x671);
ASSERT_REG_POSITION(ClearBuffers, 0x674);
ASSERT_REG_POSITION(ColorMask, 0x680);
ASSERT_REG_POSITION(Query, 0x6C0);
ASSERT_REG_POSITION(VertexArray, 0x700);
ASSERT_REG_POSITION(IndependentBlend, 0x780);
ASSERT_REG_POSITION(VertexArrayLimit, 0x7C0);
ASSERT_REG_POSITION(Firmware, 0x8C0);
ASSERT_REG_POSITION(ConstBuffer, 0x8E0);
ASSERT_REG_POSITION(CBBind, 0x904);

#undef ASSERT_REG_POSITION
