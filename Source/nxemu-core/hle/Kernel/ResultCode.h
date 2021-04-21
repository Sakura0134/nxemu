#pragma once

#include <stdint.h>

#pragma warning(push)
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union

enum ErrorModule
{
    Kernel = 1,
    IPC = 11,
    VI = 114,
    AM = 128,
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
const ResultCode ERR_INVALID_SIZE( ErrorModule::Kernel, 101 );
const ResultCode ERR_INVALID_ADDRESS(ErrorModule::Kernel, 102);
const ResultCode ERR_INVALID_ADDRESS_STATE(ErrorModule::Kernel, 106);
const ResultCode ERR_INVALID_MEMORY_PERMISSIONS(ErrorModule::Kernel, 108);
const ResultCode ERR_INVALID_HANDLE( ErrorModule::Kernel, 114 );
const ResultCode ERR_INVALID_POINTER( ErrorModule::Kernel, 115 );
const ResultCode ERR_INVALID_COMBINATION(ErrorModule::Kernel, 116);
const ResultCode RESULT_TIMEOUT(ErrorModule::Kernel, 117);
const ResultCode ERR_OUT_OF_RANGE(ErrorModule::Kernel, 119);
const ResultCode ERR_INVALID_ENUM_VALUE(ErrorModule::Kernel, 120);
const ResultCode ERR_NOT_FOUND(ErrorModule::Kernel, 121);
const ResultCode IPC_ERR_REMOTE_PROCESS_DEAD(ErrorModule::IPC, 301);
const ResultCode VI_ERR_NOT_FOUND(ErrorModule::VI, 7);
const ResultCode AM_ERR_NO_MESSAGES(ErrorModule::AM, 0x3);

const char * ResultCodeStr(ResultCode code);