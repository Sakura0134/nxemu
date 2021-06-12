#include "GpuThread.h"
#include "VideoNotification.h"
#include <Common/Util.h>

CGpuThread::CGpuThread(ISwitchSystem & SwitchSystem, CVideo & Video) : 
    CThread(stGpuThread)
{
}

CGpuThread::~CGpuThread()
{
}

bool CGpuThread::StartThread()
{
    if (isRunning())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

void CGpuThread::GpuThread(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}
