#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IApplicationFunctions :
    public CService
{
public:
    enum Method
    {
        Method_PopLaunchParameter = 1,
        Method_CreateApplicationAndPushAndRequestToStart = 10,
        Method_CreateApplicationAndPushAndRequestToStartForQuest = 11,
        Method_CreateApplicationAndRequestToStart = 12,
        Method_CreateApplicationAndRequestToStartForQuest = 13,
        Method_CreateApplicationWithAttributeAndPushAndRequestToStartForQuest = 14,
        Method_CreateApplicationWithAttributeAndRequestToStartForQuest = 15,
        Method_EnsureSaveData = 20,
        Method_GetDesiredLanguage = 21,
        Method_SetTerminateResult = 22,
        Method_GetDisplayVersion = 23,
        Method_GetLaunchStorageInfoForDebug = 24,
        Method_ExtendSaveData = 25,
        Method_GetSaveDataSize = 26,
        Method_CreateCacheStorage = 27,
        Method_BeginBlockingHomeButtonShortAndLongPressed = 30,
        Method_EndBlockingHomeButtonShortAndLongPressed = 31,
        Method_BeginBlockingHomeButton = 32,
        Method_EndBlockingHomeButton = 33,
        Method_SelectApplicationLicense = 34,
        Method_NotifyRunning = 40,
        Method_GetPseudoDeviceId = 50,
        Method_SetMediaPlaybackStateForApplication = 60,
        Method_IsGamePlayRecordingSupported = 65,
        Method_InitializeGamePlayRecording = 66,
        Method_SetGamePlayRecordingState = 67,
        Method_RequestFlushGamePlayingMovieForDebug = 68,
        Method_RequestToShutdown = 70,
        Method_RequestToReboot = 71,
        Method_RequestToSleep = 72,
        Method_ExitAndRequestToShowThanksMessage = 80,
        Method_EnableApplicationCrashReport = 90,
        Method_InitializeApplicationCopyrightFrameBuffer = 100,
        Method_SetApplicationCopyrightImage = 101,
        Method_SetApplicationCopyrightVisibility = 102,
        Method_QueryApplicationPlayStatistics = 110,
        Method_QueryApplicationPlayStatisticsByUid = 111,
        Method_ExecuteProgram = 120,
        Method_ClearUserChannel = 121,
        Method_UnpopToUserChannel = 122,
        Method_GetPreviousProgramIndex = 123,
        Method_EnableApplicationAllThreadDumpOnCrash = 124,
        Method_GetGpuErrorDetectedSystemEvent = 130,
        Method_GetFriendInvitationStorageChannelEvent = 140,
        Method_TryPopFromFriendInvitationStorageChannel = 141,
        Method_GetNotificationStorageChannelEvent = 150,
        Method_TryPopFromNotificationStorageChannel = 151,
        Method_GetHealthWarningDisappearedSystemEvent = 160,
        Method_SetHdcpAuthenticationActivated = 170,
        Method_StartContinuousRecordingFlushForDebug = 500,
        Method_CreateMovieMaker = 1000,
        Method_PrepareForJit = 1001,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IApplicationFunctions"; }
    
private:
    IApplicationFunctions(void);
    IApplicationFunctions(const IApplicationFunctions&);
    IApplicationFunctions& operator=(const IApplicationFunctions&);

    IApplicationFunctions(CSwitchSystem & System);

	void ProcessPopLaunchParameter(CIPCRequest & Request);
	void ProcessEnsureSaveData(CIPCRequest & Request);
	void ProcessNotifyRunning(CIPCRequest & Request);
};
