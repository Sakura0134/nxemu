#include <nxemu-core\hle\kernel\ResultCode.h>
#include <Common\StdString.h>

ResultCode::ResultCode(uint32_t _Raw) :
    Raw(_Raw)
{
}

ResultCode::ResultCode(ErrorModule _Module, uint32_t _Description) :
    Module(_Module),
    Description(_Description)
{
}

bool ResultCode::operator == (const ResultCode& rCode) const
{
    return Raw == rCode.Raw;
}

bool ResultCode::IsSuccess() const
{
    return Raw == 0;
}

bool ResultCode::IsError() const
{
    return Raw != 0;
}

const char * ResultCodeStr(ResultCode code)
{
    if (code == RESULT_SUCCESS) { return "RESULT_SUCCESS"; }

    static stdstr unknown;
    unknown.Format("Unknown (0x%X)", code);
    return unknown.c_str();
}