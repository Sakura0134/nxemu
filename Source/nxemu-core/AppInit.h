#pragma once

#include <nxemu-core/Notification.h>

bool AppInit(CNotification * Notify, const char * BaseDirectory);
void AppCleanup(void);
