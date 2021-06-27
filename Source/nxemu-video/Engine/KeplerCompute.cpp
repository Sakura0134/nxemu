#include "Engine/KeplerCompute.h"
#include "VideoNotification.h"

CKeplerCompute::CKeplerCompute()
{
}

CKeplerCompute::~CKeplerCompute() 
{
}

void CKeplerCompute::CallMethod(uint32_t Method, uint32_t Argument, bool Last)
{
    if (Method >= NumRegisters) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_Regs.Value[Method] = Argument;

    switch (Method) 
    {
    case Method_ExecUpload:
    case Method_DataUpload:
    case Method_Launch:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
}

void CKeplerCompute::CallMultiMethod(uint32_t Method, const uint32_t * BaseStart, uint32_t Amount, uint32_t MethodsPending) 
{
    for (uint32_t i = 0; i < Amount; i++)
    {
        CallMethod(Method, BaseStart[i], MethodsPending - i <= 1);
    }
}
