#pragma once
#include <stdint.h>
#include <Common/Padding.h>

enum ShaderOutputTopology : uint32_t
{
    ShaderOutputTopology_PointList = 1,
    ShaderOutputTopology_LineStrip = 6,
    ShaderOutputTopology_TriangleStrip = 7,
};

enum ShaderPixelImap : uint8_t 
{
    ShaderPixelImap_Unused = 0,
    ShaderPixelImap_Constant = 1,
    ShaderPixelImap_Perspective = 2,
    ShaderPixelImap_ScreenLinear = 3,
};

#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
struct ShaderHeader
{
    typedef struct 
    {
        unsigned SphType : 5;
        unsigned Version : 5;
        unsigned ShaderType : 4;
        unsigned MrtEnable : 1;
        unsigned KillsPixels : 1;
        unsigned DoesGlobalStore : 1;
        unsigned SassVersion : 4;
        unsigned Reserved : 5;
        unsigned DoesLoadOrStore : 1;
        unsigned DoesFp64 : 1;
        unsigned StreamOutMask : 4;
    } tyCommonWord0;

    typedef struct 
    {
        unsigned ShaderLocalMemoryLowSize : 24;
        unsigned PerPatchAttributeCount : 8;
    } tyCommonWord1;

    typedef struct 
    {
        unsigned ShaderLocalMemoryHighSize : 24;
        unsigned ThreadsPerInputPrimitive : 8;
    } tyCommonWord2;

    typedef struct 
    {
        unsigned ShaderLocalMemoryCrsSize : 24;
        ShaderOutputTopology OutputTopology : 4;
        unsigned Reserved : 4;
    } tyCommonWord3;

    typedef struct 
    {
        unsigned MaxOutputVertices : 12;
        unsigned StoreReqStart : 8;
        unsigned Reserved : 4;
        unsigned StoreReqEnd : 8;
    } tyCommonWord4;

    typedef struct 
    {
        uint16_t ClipDistances : 8;
        uint16_t point_sprite_s : 1;
        uint16_t point_sprite_t : 1;
        uint16_t fog_coordinate : 1;
        uint16_t : 1;
        uint16_t tessellation_eval_point_u : 1;
        uint16_t tessellation_eval_point_v : 1;
        uint16_t instance_id : 1;
        uint16_t vertex_id : 1;
    } tyImapSystemValuesC;

    typedef union 
    {
        ShaderPixelImap ImapX : 2;
        ShaderPixelImap ImapY : 2;
        ShaderPixelImap ImapZ : 2;
        ShaderPixelImap ImapW : 2;
    } tyImapPixelVector;

    typedef struct
    {
        tyCommonWord0 CommonWord0;
        tyCommonWord1 CommonWord1;
        tyCommonWord2 CommonWord2;
        tyCommonWord3 CommonWord3;
        tyCommonWord4 CommonWord4;
        PADDING_BYTES(3);  // ImapSystemValuesA
        PADDING_BYTES(1);  // ImapSystemValuesB
        PADDING_BYTES(16); // ImapGenericVector[32]
        PADDING_BYTES(2);  // ImapColor
        tyImapSystemValuesC ImapSystemValuesC;
        PADDING_BYTES(5);  // ImapFixedFncTexture[10]
        PADDING_BYTES(1);  // ImapReserved
        PADDING_BYTES(3);  // OmapSystemValuesA
        PADDING_BYTES(1);  // OmapSystemValuesB
        PADDING_BYTES(16); // OmapGenericVector[32]
        PADDING_BYTES(2);  // OmapColor
        PADDING_BYTES(2);  // OmapSystemValuesC
        PADDING_BYTES(5);  // OmapFixedFncTexture[10]
        PADDING_BYTES(1);  // OmapReserved
    } tyVTG;
    
    typedef struct
    {
        tyCommonWord0 CommonWord0;
        tyCommonWord1 CommonWord1;
        tyCommonWord2 CommonWord2;
        tyCommonWord3 CommonWord3;
        tyCommonWord4 CommonWord4;
        PADDING_BYTES(3); // ImapSystemValuesA
        PADDING_BYTES(1); // ImapSystemValuesB
        tyImapPixelVector ImapGenericVector[32];
        PADDING_BYTES(2);  // ImapColor
        PADDING_BYTES(2);  // ImapSystemValuesC
        PADDING_BYTES(10); // ImapFixedFncTexture[10]
        PADDING_BYTES(2);  // ImapReserved
        uint32_t OmapTarget;
        struct 
        {
            unsigned OmapSampleMask : 1;
            unsigned OmapDepth : 1;
            unsigned : 30; //OmapReserved
        };
    } tyPS;

    union 
    {
        tyVTG VTG;
        tyPS PS;
    };

    uint64_t GetLocalMemorySize() const;
    ShaderPixelImap GetPixelImap(uint32_t Attribute) const;
    bool IsColorComponentOutputEnabled(uint32_t RenderTarget, uint32_t Component) const;
};
#pragma warning(pop)
