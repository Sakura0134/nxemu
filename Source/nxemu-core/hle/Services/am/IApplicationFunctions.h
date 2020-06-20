#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IApplicationFunctions :
    public CService
{
public:
    enum Method
    {
        PopLaunchParameter = 1,
        CreateApplicationAndPushAndRequestToStart = 10,
        CreateApplicationAndPushAndRequestToStartForQuest = 11,
        CreateApplicationAndRequestToStart = 12,
        CreateApplicationAndRequestToStartForQuest = 13,
        CreateApplicationWithAttributeAndPushAndRequestToStartForQuest = 14,
        CreateApplicationWithAttributeAndRequestToStartForQuest = 15,
        EnsureSaveData = 20,
        GetDesiredLanguage = 21,
        SetTerminateResult = 22,
        GetDisplayVersion = 23,
        GetLaunchStorageInfoForDebug = 24,
        ExtendSaveData = 25,
        GetSaveDataSize = 26,
        CreateCacheStorage = 27,
        BeginBlockingHomeButtonShortAndLongPressed = 30,
        EndBlockingHomeButtonShortAndLongPressed = 31,
        BeginBlockingHomeButton = 32,
        EndBlockingHomeButton = 33,
        SelectApplicationLicense = 34,
		NotifyRunning = 40,
        GetPseudoDeviceId = 50,
        SetMediaPlaybackStateForApplication = 60,
        IsGamePlayRecordingSupported = 65,
		InitializeGamePlayRecording = 66,
        SetGamePlayRecordingState = 67,
        RequestFlushGamePlayingMovieForDebug = 68,
        RequestToShutdown = 70,
        RequestToReboot = 71,
        RequestToSleep = 72,
        ExitAndRequestToShowThanksMessage = 80,
        EnableApplicationCrashReport = 90,
        InitializeApplicationCopyrightFrameBuffer = 100,
        SetApplicationCopyrightImage = 101,
        SetApplicationCopyrightVisibility = 102,
        QueryApplicationPlayStatistics = 110,
        QueryApplicationPlayStatisticsByUid = 111,
        ExecuteProgram = 120,
        ClearUserChannel = 121,
        UnpopToUserChannel = 122,
        GetPreviousProgramIndex = 123,
        EnableApplicationAllThreadDumpOnCrash = 124,
        GetGpuErrorDetectedSystemEvent = 130,
        GetFriendInvitationStorageChannelEvent = 140,
        TryPopFromFriendInvitationStorageChannel = 141,
        GetNotificationStorageChannelEvent = 150,
        TryPopFromNotificationStorageChannel = 151,
        GetHealthWarningDisappearedSystemEvent = 160,
        SetHdcpAuthenticationActivated = 170,
        StartContinuousRecordingFlushForDebug = 500,
        CreateMovieMaker = 1000,
        PrepareForJit = 1001,
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
};
