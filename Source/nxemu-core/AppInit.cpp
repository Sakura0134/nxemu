#include "AppInit.h"

bool AppInit(CNotification * Notify, const char * BaseDirectory, int argc, char **argv)
{
    g_Notify = Notify;
    return true;
}

void AppCleanup(void)
{
}