#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class ISelfController :
    public CService
{
public:
    enum Method
    {
        Exit = 0,
        LockExit = 1,
        UnlockExit = 2,
        EnterFatalSection = 3,
        LeaveFatalSection = 4,
        GetLibraryAppletLaunchableEvent = 9,
        SetScreenShotPermission = 10,
        SetOperationModeChangedNotification = 11,
        SetPerformanceModeChangedNotification = 12,
        SetFocusHandlingMode = 13,
        SetRestartMessageEnabled = 14,
        SetScreenShotAppletIdentityInfo = 15,
        SetOutOfFocusSuspendingEnabled = 16,
        SetControllerFirmwareUpdateSection = 17,
        SetRequiresCaptureButtonShortPressedMessage = 18,
        SetAlbumImageOrientation = 19,
        SetDesirableKeyboardLayout = 20,
        CreateManagedDisplayLayer = 40,
        IsSystemBufferSharingEnabled = 41,
        GetSystemSharedLayerHandle = 42,
        GetSystemSharedBufferHandle = 43,
        CreateManagedDisplaySeparableLayer = 44,
        SetManagedDisplayLayerSeparationMode = 45,
        SetHandlesRequestToDisplay = 50,
        ApproveToDisplay = 51,
        OverrideAutoSleepTimeAndDimmingTime = 60,
        SetMediaPlaybackState = 61,
        SetIdleTimeDetectionExtension = 62,
        GetIdleTimeDetectionExtension = 63,
        SetInputDetectionSourceSet = 64,
        ReportUserIsActive = 65,
        GetCurrentIlluminance = 66,
        IsIlluminanceAvailable = 67,
        SetAutoSleepDisabled = 68,
        IsAutoSleepDisabled = 69,
        ReportMultimediaError = 70,
        GetCurrentIlluminanceEx = 71,
        SetInputDetectionPolicy = 72,
        SetWirelessPriorityMode = 80,
        GetAccumulatedSuspendedTickValue = 90,
        GetAccumulatedSuspendedTickChangedEvent = 91,
        SetAlbumImageTakenNotificationEnabled= 100,
        SetApplicationAlbumUserData = 110,
        GetDebugStorageChannel = 1000,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "ISelfController"; }

private:
    ISelfController(void);
    ISelfController(const ISelfController&);
    ISelfController& operator=(const ISelfController&);

    ISelfController(CSwitchSystem & System);
};
