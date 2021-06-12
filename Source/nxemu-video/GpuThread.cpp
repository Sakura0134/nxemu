#include "GpuThread.h"
#include "VideoNotification.h"

CGpuThread::CGpuThread(ISwitchSystem & SwitchSystem, CVideo & Video) : 
    CThread(stGpuThread)
{
}

CGpuThread::~CGpuThread()
{
}

void CGpuThread::GpuThread(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}
