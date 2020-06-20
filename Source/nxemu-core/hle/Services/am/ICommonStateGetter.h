#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class ICommonStateGetter :
    public CService
{
public:
    enum Method
    {
        GetEventHandle = 0,
        ReceiveMessage = 1,
        GetThisAppletKind = 2,
        AllowToEnterSleep = 3,
        DisallowToEnterSleep = 4,
        GetOperationMode = 5,
        GetPerformanceMode = 6,
        GetCradleStatus = 7,
        GetBootMode = 8,
        GetCurrentFocusState = 9,
        RequestToAcquireSleepLock = 10,
        ReleaseSleepLock = 11,
        ReleaseSleepLockTransiently = 12,
        GetAcquiredSleepLockEvent = 13,
        PushToGeneralChannel = 20,
        GetHomeButtonReaderLockAccessor = 30,
        GetReaderLockAccessorEx = 31,
        GetWriterLockAccessorEx = 32,
        GetCradleFwVersion = 40,
        IsVrModeEnabled = 50,
        SetVrModeEnabled = 51,
        SetLcdBacklighOffEnabled = 52,
        BeginVrModeEx = 53,
        EndVrModeEx = 54,
        IsInControllerFirmwareUpdateSection = 55,
        GetDefaultDisplayResolution = 60,
        GetDefaultDisplayResolutionChangeEvent = 61,
        GetHdcpAuthenticationState = 62,
        GetHdcpAuthenticationStateChangeEvent = 63,
        SetTvPowerStateMatchingMode = 64,
        GetApplicationIdByContentActionName = 65,
        SetCpuBoostMode = 66,
        CancelCpuBoostMode = 67,
        PerformSystemButtonPressingIfInFocus = 80,
        SetPerformanceConfigurationChangedNotification = 90,
        GetCurrentPerformanceConfiguration = 91,
        SetHandlingHomeButtonShortPressedEnabled = 100,
        GetOperationModeSystemInfo = 200,
        GetSettingsPlatformRegion = 300,
        ActivateMigrationService = 400,
        DeactivateMigrationService = 401,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "ICommonStateGetter"; }

private:
    ICommonStateGetter(void);
    ICommonStateGetter(const ICommonStateGetter&);
    ICommonStateGetter& operator=(const ICommonStateGetter&);

    ICommonStateGetter(CSwitchSystem & System);
};
