#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class ISelfController :
    public CService
{
public:
    enum Method
    {
        Method_Exit = 0,
        Method_LockExit = 1,
        Method_UnlockExit = 2,
        Method_EnterFatalSection = 3,
        Method_LeaveFatalSection = 4,
        Method_GetLibraryAppletLaunchableEvent = 9,
        Method_SetScreenShotPermission = 10,
        Method_SetOperationModeChangedNotification = 11,
        Method_SetPerformanceModeChangedNotification = 12,
        Method_SetFocusHandlingMode = 13,
        Method_SetRestartMessageEnabled = 14,
        Method_SetScreenShotAppletIdentityInfo = 15,
        Method_SetOutOfFocusSuspendingEnabled = 16,
        Method_SetControllerFirmwareUpdateSection = 17,
        Method_SetRequiresCaptureButtonShortPressedMessage = 18,
        Method_SetAlbumImageOrientation = 19,
        Method_SetDesirableKeyboardLayout = 20,
        Method_CreateManagedDisplayLayer = 40,
        Method_IsSystemBufferSharingEnabled = 41,
        Method_GetSystemSharedLayerHandle = 42,
        Method_GetSystemSharedBufferHandle = 43,
        Method_CreateManagedDisplaySeparableLayer = 44,
        Method_SetManagedDisplayLayerSeparationMode = 45,
        Method_SetHandlesRequestToDisplay = 50,
        Method_ApproveToDisplay = 51,
        Method_OverrideAutoSleepTimeAndDimmingTime = 60,
        Method_SetMediaPlaybackState = 61,
        Method_SetIdleTimeDetectionExtension = 62,
        Method_GetIdleTimeDetectionExtension = 63,
        Method_SetInputDetectionSourceSet = 64,
        Method_ReportUserIsActive = 65,
        Method_GetCurrentIlluminance = 66,
        Method_IsIlluminanceAvailable = 67,
        Method_SetAutoSleepDisabled = 68,
        Method_IsAutoSleepDisabled = 69,
        Method_ReportMultimediaError = 70,
        Method_GetCurrentIlluminanceEx = 71,
        Method_SetInputDetectionPolicy = 72,
        Method_SetWirelessPriorityMode = 80,
        Method_GetAccumulatedSuspendedTickValue = 90,
        Method_GetAccumulatedSuspendedTickChangedEvent = 91,
        Method_SetAlbumImageTakenNotificationEnabled= 100,
        Method_SetApplicationAlbumUserData = 110,
        Method_GetDebugStorageChannel = 1000,
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

    ResultCode ProcessCreateManagedDisplayLayer(CIPCRequest & Request);
};
