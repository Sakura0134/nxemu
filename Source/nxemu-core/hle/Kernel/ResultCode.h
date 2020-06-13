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
const ResultCode ERR_INVALID_HANDLE( ErrorModule::Kernel, 114 );
const ResultCode ERR_INVALID_COMBINATION(ErrorModule::Kernel, 116);
const ResultCode ERR_OUT_OF_RANGE(ErrorModule::Kernel, 119);
const ResultCode ERR_INVALID_ENUM_VALUE(ErrorModule::Kernel, 120);
const ResultCode ERR_NOT_FOUND(ErrorModule::Kernel, 121);

const char * ResultCodeStr(ResultCode code);