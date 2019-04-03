#pragma once
#include <nxemu-core/Notification.h>

class CMainGui;

class CNotificationImp :
    public CNotification
{
public:
    CNotificationImp(void);

    virtual void DisplayError(const char * Message) const;
    virtual void BreakPoint(const char * FileName, uint32_t LineNumber);
    virtual void AppInitDone(void);

    //User Feedback
    virtual void DisplayMessage(uint32_t DisplayTime, const char * Message) const;
    virtual void DisplayMessage(uint32_t DisplayTime, LanguageStringID StringID) const;

	//Switch Keys
	virtual bool GetSwitchKeys(CSwitchKeys * keys) const;

    void SetMainWindow(CMainGui * Gui);

private:
    CNotificationImp(const CNotificationImp&);            // Disable copy constructor
    CNotificationImp& operator=(const CNotificationImp&); // Disable assignment

    CMainGui * m_Gui;
    mutable time_t m_NextMsg;
};

CNotificationImp & Notify(void);
