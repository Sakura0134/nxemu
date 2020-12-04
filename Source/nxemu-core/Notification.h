#pragma once
#include <Common/stdtypes.h>
#include <nxemu-core/Language/Language.h>

#ifndef _MSC_VER
#define __interface struct
#endif

class CSwitchKeys;

__interface CNotification
{
public:
    //Error Messages
    virtual void DisplayError(const char * Message) const = 0;
    virtual void BreakPoint(const char * FileName, uint32_t LineNumber) = 0;

    //User Feedback
    virtual void DisplayMessage(const char * Message) const = 0;
    virtual void DisplayMessageTime(uint32_t DisplayTime, const char * Message) const = 0;
    virtual void DisplayMessageTime(uint32_t DisplayTime, LanguageStringID StringID) const = 0;

	//Switch Keys
	virtual bool GetSwitchKeys(CSwitchKeys * keys) const = 0;

	virtual void AppInitDone(void) = 0;
};

extern CNotification * g_Notify;
