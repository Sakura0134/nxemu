#include "Engine/Maxwell3D.h"
#include "VideoNotification.h"

CMaxwell3D::CMaxwell3D(ISwitchSystem & SwitchSystem, CVideoMemory & VideoMemory) :
    m_SwitchSystem(SwitchSystem),
    m_VideoMemory(VideoMemory)
{
}

CMaxwell3D::~CMaxwell3D()
{
}

void CMaxwell3D::CallMethod(Method Method, uint32_t Argument, bool Last)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}
