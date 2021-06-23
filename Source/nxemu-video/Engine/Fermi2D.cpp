#include "Fermi2D.h"
#include "VideoNotification.h"

CFermi2D::CFermi2D()
{
}

CFermi2D::~CFermi2D()
{
}

void CFermi2D::CallMethod(uint32_t Method, uint32_t Argument, bool /*Last*/) 
{
    if (Method >= NumRegisters)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_Regs.Value[Method] = Argument;

    if (Method == Method_PixelsFromMemorySrcY0Low) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}
