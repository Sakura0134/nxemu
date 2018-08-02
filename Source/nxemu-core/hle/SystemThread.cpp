#include <nxemu-core\hle\SystemThread.h>
#include <nxemu-core\SystemGlobals.h>

CSystemThread::~CSystemThread()
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}