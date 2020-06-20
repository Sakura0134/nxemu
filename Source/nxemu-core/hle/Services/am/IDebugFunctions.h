#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IDebugFunctions :
    public CService
{
public:
    enum Method
    {
        NotifyMessageToHomeMenuForDebug = 0,
        OpenMainApplication = 1,
        PerformSystemButtonPressing = 10,
        InvalidateTransitionLayer = 20,
        RequestLaunchApplicationWithUserAndArgumentForDebug = 30,
        GetAppletResourceUsageInfo = 40,
        SetCpuBoostModeForApplet = 100,
        CancelCpuBoostModeForApplet = 101,
        PushToAppletBoundChannelForDebug = 110,
        TryPopFromAppletBoundChannelForDebug = 111,
        AlarmSettingNotificationEnableAppEventReserve = 120,
        AlarmSettingNotificationDisableAppEventReserve = 121,
        AlarmSettingNotificationPushAppEventNotify = 122,
        FriendInvitationSetApplicationParameter = 130,
        FriendInvitationClearApplicationParameter = 131,
        FriendInvitationPushApplicationParameter = 132,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IDebugFunctions"; }

private:
    IDebugFunctions(void);
    IDebugFunctions(const IDebugFunctions&);
    IDebugFunctions& operator=(const IDebugFunctions&);

    IDebugFunctions(CSwitchSystem & System);
};
