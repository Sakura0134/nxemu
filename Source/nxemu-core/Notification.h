#pragma once
#include <Common/stdtypes.h>

#ifndef _MSC_VER
#define __interface struct
#endif

__interface CNotification
{
public:
    //Error Messages
    virtual void DisplayError(const char * Message) const = 0;
    virtual void BreakPoint(const char * FileName, uint32_t LineNumber) = 0;

    virtual void AppInitDone(void) = 0;
};

extern CNotification * g_Notify;
