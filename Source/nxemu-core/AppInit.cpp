#include "AppInit.h"

bool AppInit(CNotification * Notify, const char * BaseDirectory, int argc, char **argv)
{
    g_Notify = Notify;
    g_Notify->AppInitDone();
    return true;
}

void AppCleanup(void)
{
}