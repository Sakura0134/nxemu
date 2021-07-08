#pragma once
#include "GpuTypes.h"
#include "Engine\UploadState.h"
#include "Macro\MacroEngine.h"
#include "Util\StateTracker.h"
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
        NumCBData = 16,
        MaxShaderStage = 5,
        MaxConstBuffers = 18,
    };

    enum QueryOperation : unsigned
    {
        QueryOperation_Release = 0,
        QueryOperation_Acquire = 1,
        QueryOperation_Counter = 2,
        QueryOperation_Trap = 3,
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

    enum ShadowRamControl : unsigned
    {
        ShadowRamControl_Track = 0,
        ShadowRamControl_TrackWithFilter = 1,
        ShadowRamControl_Passthrough = 2,
        ShadowRamControl_Replay = 3,
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

    enum IndexFormat : unsigned
    {
        IndexFormat_UnsignedByte = 0x0,
        IndexFormat_UnsignedShort = 0x1,
        IndexFormat_UnsignedInt = 0x2,
    };

#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
#pragma warning(disable : 5208) // warning C5208: unnamed class used in typedef name cannot declare members other than non-static
    typedef struct
    {
        uint64_t Address;
        uint32_t Size;
        bool Enabled;
    } ConstBuffer;

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

    typedef struct _tyCondition
    {
        uint32_t AddressHigh;
        uint32_t AddressLow;
        ConditionMode Mode;

        uint64_t Address(void) const
        {
            return (((uint64_t)AddressHigh) << 32) | AddressLow;
        }
    } tyCondition;

    typedef struct _tyConstBuffer
    {
        uint32_t Size;
        uint32_t AddressHigh;
        uint32_t AddressLow;
        uint32_t Pos;
        uint32_t Data[NumCBData];

        uint64_t Address() const
        {
            return (((uint64_t)AddressHigh) << 32) | AddressLow;
        }
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
        uint32_t UploadAddress;
        uint32_t Data;
        uint32_t Entry;
        uint32_t Bind;
    } tyMacros;

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

        uint64_t Address() const
        {
            return (((uint64_t)AddressHigh) << 32) | AddressLow;
        }
    } tyQuery;

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

    typedef struct _tyIndexArray
    {
        uint32_t StartAddrHigh;
        uint32_t StartAddrLow;
        uint32_t EndAddrHigh;
        uint32_t EndAddrLow;
        IndexFormat Format;
        uint32_t First;
        uint32_t Count;
    } tyIndexArray;

    union tyColorMask
    {
        uint32_t Value;
        struct 
        {
            unsigned R : 4;
            unsigned G : 4;
            unsigned B : 4;
            unsigned A : 4;
        };
    };

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
            PADDING_WORDS(0x2AB);
            uint32_t VertexBufferCount;
            PADDING_WORDS(0x19);
            uint32_t FragmentBarrier;
            PADDING_WORDS(0x66);
            uint32_t TiledCacheBarrier;
            PADDING_WORDS(0x4);
            uint32_t ColorMaskCommon;
            PADDING_WORDS(0x105);
            uint32_t FragmentColorClamp;
            PADDING_WORDS(0x61);
            CounterReset CounterReset;
            PADDING_WORDS(0x7);
            tyCondition Condition;
            PADDING_WORDS(0x2E);
            tyDraw Draw;
            PADDING_WORDS(0x6B);
            tyIndexArray IndexArray;
            PADDING_WORDS(0x7B);
            tyClearBuffers ClearBuffers;
            PADDING_WORDS(0xB);
            tyColorMask ColorMask[NumRenderTargets];
            PADDING_WORDS(0x38);
            tyQuery Query;
            PADDING_WORDS(0x1FC);
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
        Method_CBBind0 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 0)) / sizeof(uint32_t),
        Method_CBBind1 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 1)) / sizeof(uint32_t),
        Method_CBBind2 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 2)) / sizeof(uint32_t),
        Method_CBBind3 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 3)) / sizeof(uint32_t),
        Method_CBBind4 = (offsetof(Registers, CBBind) + (sizeof(Registers::CBBind[0]) * 4)) / sizeof(uint32_t),
        Method_ClearBuffers = offsetof(Registers, ClearBuffers) / sizeof(uint32_t),
        Method_ColorMask = offsetof(Registers, ColorMask) / sizeof(uint32_t),
        Method_ColorMaskCommon = offsetof(Registers, ColorMaskCommon) / sizeof(uint32_t),
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
        Method_ConditionMode = offsetof(Registers, Condition.Mode) / sizeof(uint32_t),
        Method_CounterReset = offsetof(Registers, CounterReset) / sizeof(uint32_t),
        Method_DataUpload = offsetof(Registers, DataUpload) / sizeof(uint32_t),
        Method_DrawVertexBeginGL = offsetof(Registers, Draw.VertexBeginGL) / sizeof(uint32_t),
        Method_DrawVertexEndGL = offsetof(Registers, Draw.VertexEndGL) / sizeof(uint32_t),
        Method_ExecUpload = offsetof(Registers, ExecUpload) / sizeof(uint32_t),
        Method_Firmware4 = (offsetof(Registers, Firmware) + (sizeof(Registers::Firmware[0]) * 4)) / sizeof(uint32_t),
        Method_FragmentBarrier = offsetof(Registers, FragmentBarrier) / sizeof(uint32_t),
        Method_FragmentColorClamp = offsetof(Registers, FragmentColorClamp) / sizeof(uint32_t),
        Method_IndexArrayCount = offsetof(Registers, IndexArray.Count) / sizeof(uint32_t),
        Method_QueryGet = offsetof(Registers, Query.QueryGet) / sizeof(uint32_t),
        Method_MacrosBind = offsetof(Registers, Macros.Bind) / sizeof(uint32_t),
        Method_MacrosData = offsetof(Registers, Macros.Data) / sizeof(uint32_t),
        Method_ShadowRamControl = offsetof(Registers, ShadowRamControl) / sizeof(uint32_t),
        Method_SyncInfo = offsetof(Registers, SyncInfo) / sizeof(uint32_t),
        Method_TiledCacheBarrier = offsetof(Registers, TiledCacheBarrier) / sizeof(uint32_t),
        Method_VertexBufferCount = offsetof(Registers, VertexBufferCount) / sizeof(uint32_t),
        Method_WaitForIdle = offsetof(Registers, WaitForIdle) / sizeof(uint32_t),
    };

    CMaxwell3D(ISwitchSystem & SwitchSystem, CVideoMemory & VideoMemory);
    ~CMaxwell3D();

    void BindRenderer(IRenderer * Renderer);
    void CallMultiMethod(Method Method, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending);
    void CallMethodFromMME(Method Method, uint32_t Argument);
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

    ISwitchSystem & m_SwitchSystem;
    CVideoMemory & m_VideoMemory;
    IRenderer * m_Renderer;
    Registers m_Regs, m_ShadowRegs;
    uint32_t m_MacroPositions[0x80];
    std::unique_ptr<MacroEngine> m_MacroEngine;
    CBDataState m_CBDataState;
    uint32_t m_ExecutingMacro;
    MacroParams m_MacroParams;
    CUploadState m_UploadState;
    CStateTracker m_StateTracker;
    ConstBuffer m_ShaderStage[MaxShaderStage][MaxConstBuffers];

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
ASSERT_REG_POSITION(VertexBufferCount, 0x35E);
ASSERT_REG_POSITION(FragmentBarrier, 0x378);
ASSERT_REG_POSITION(TiledCacheBarrier, 0x3DF);
ASSERT_REG_POSITION(ColorMaskCommon, 0x3E4);
ASSERT_REG_POSITION(FragmentColorClamp, 0x4EA);
ASSERT_REG_POSITION(CounterReset, 0x54C);
ASSERT_REG_POSITION(Condition, 0x554);
ASSERT_REG_POSITION(Draw, 0x585);
ASSERT_REG_POSITION(IndexArray, 0x5F2);
ASSERT_REG_POSITION(ClearBuffers, 0x674);
ASSERT_REG_POSITION(ColorMask, 0x680);
ASSERT_REG_POSITION(Query, 0x6C0);
ASSERT_REG_POSITION(Firmware, 0x8C0);
ASSERT_REG_POSITION(ConstBuffer, 0x8E0);
ASSERT_REG_POSITION(CBBind, 0x904);

#undef ASSERT_REG_POSITION
