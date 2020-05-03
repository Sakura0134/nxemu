#pragma once

#include <Common\stdtypes.h>

#pragma warning(push)
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union

enum ErrorModule : uint32_t
{
    Kernel = 1,
};

union ResultCode
{
    ResultCode(uint32_t _Raw);
    ResultCode(ErrorModule _Module, uint32_t _Description);

    bool operator == (const ResultCode& rCode) const;
    bool IsSuccess() const;
    bool IsError() const;

    uint32_t Raw;
    struct 
    {
        unsigned Module : 9;
        unsigned Description : 12;
    };
};

#pragma warning(pop)

const ResultCode RESULT_SUCCESS(0);

const char * ResultCodeStr(ResultCode code);