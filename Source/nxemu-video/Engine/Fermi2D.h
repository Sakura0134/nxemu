#pragma once
#include <Common/padding.h>
#include <stdint.h>
#include <cstddef>

class CFermi2D
{
public:
    enum 
    {
        NumRegisters = 0x258,
    };

    typedef struct 
    {
        uint32_t BlockShape;
        uint32_t CorralSize;
        uint32_t SafeOverlap;
        uint32_t SampleMode;
        PADDING_WORDS(0x8);
        int32_t DstX0;
        int32_t DstY0;
        int32_t DstWidth;
        int32_t DstHeight;
        int64_t DuDx;
        int64_t DvDy;
        int64_t SrcX0;
        int64_t SrcY0;
    } PixelsFromMemory;

    union Registers 
    {
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
        struct 
        {
            PADDING_WORDS(0x220);
            PixelsFromMemory PixelsFromMemory;
        };
#pragma warning(pop)
        uint32_t Value[NumRegisters];
    };

    enum Method : uint32_t 
    {
        Method_PixelsFromMemorySrcY0Low = (offsetof(Registers, PixelsFromMemory.SrcY0) / sizeof(uint32_t)) + 1,
    };

    CFermi2D();
    ~CFermi2D();

    void CallMethod(uint32_t Method, uint32_t Argument, bool Last);

private:
    CFermi2D(const CFermi2D&);
    CFermi2D& operator=(const CFermi2D&);

    Registers m_Regs;
};

#define ASSERT_REG_POSITION(field_name, position) static_assert(offsetof(CFermi2D::Registers, field_name) == position, "Field " #field_name " has invalid position")

ASSERT_REG_POSITION(PixelsFromMemory, 0x880);
ASSERT_REG_POSITION(PixelsFromMemory.SrcY0, 0x8D8);

#undef ASSERT_REG_POSITION
