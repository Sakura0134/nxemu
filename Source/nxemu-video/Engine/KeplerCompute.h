#pragma once
#include <stdint.h>
#include <cstddef>
#include "Common/Padding.h"

class CKeplerCompute 
{
public:
    enum
    {
        NumRegisters = 0xCF8,
    };

    union Registers
    {
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
        struct 
        {
            PADDING_WORDS(0x6C);
            uint32_t ExecUpload;
            uint32_t DataUpload;
            PADDING_WORDS(0x41);
            uint32_t Launch;
        };
#pragma warning(pop)
        uint32_t Value[NumRegisters];
    };

    enum Method : uint32_t 
    {
        Method_ExecUpload = offsetof(Registers, ExecUpload) / sizeof(uint32_t),
        Method_DataUpload = offsetof(Registers, DataUpload) / sizeof(uint32_t),
        Method_Launch = offsetof(Registers, Launch) / sizeof(uint32_t),
    };

    CKeplerCompute();
    ~CKeplerCompute();

    void CallMethod(uint32_t Method, uint32_t Argument, bool Last);
    void CallMultiMethod(uint32_t Method, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending);

private:
    CKeplerCompute(const CKeplerCompute&);
    CKeplerCompute& operator=(const CKeplerCompute&);

    Registers m_Regs;
};

#define ASSERT_REG_POSITION(field_name, position) static_assert(offsetof(CKeplerCompute::Registers, field_name) == position * 4, "Field " #field_name " has invalid position")

ASSERT_REG_POSITION(ExecUpload, 0x6C);
ASSERT_REG_POSITION(DataUpload, 0x6D);
ASSERT_REG_POSITION(Launch, 0xAF);

#undef ASSERT_REG_POSITION
