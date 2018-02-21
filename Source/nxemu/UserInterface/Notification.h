#pragma once
#include <nxemu-core/Notification.h>

class CNotificationImp :
    public CNotification
{
public:
    CNotificationImp(void);

    virtual void DisplayError(const char * Message) const;
    virtual void BreakPoint(const char * FileName, uint32_t LineNumber);
    virtual void AppInitDone(void);
private:
    CNotificationImp(const CNotificationImp&);            // Disable copy constructor
    CNotificationImp& operator=(const CNotificationImp&); // Disable assignment
};

CNotificationImp & Notify(void);
