#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class HID :
    public CService
{
public:
    enum Method
    {
        Method_CreateAppletResource = 0,
        Method_ActivateDebugPad = 1,
        Method_ActivateTouchScreen = 11,
        Method_ActivateMouse = 21,
        Method_ActivateKeyboard = 31,
        Method_SendKeyboardLockKeyEvent = 32,
        Method_AcquireXpadIdEventHandle = 40,
        Method_ReleaseXpadIdEventHandle = 41,
        Method_ActivateXpad = 51,
        Method_GetXpadIds = 55,
        Method_ActivateJoyXpad = 56,
        Method_GetJoyXpadLifoHandle = 58,
        Method_GetJoyXpadIds = 59,
        Method_ActivateSixAxisSensor = 60,
        Method_DeactivateSixAxisSensor = 61,
        Method_GetSixAxisSensorLifoHandle = 62,
        Method_ActivateJoySixAxisSensor = 63,
        Method_DeactivateJoySixAxisSensor = 64,
        Method_GetJoySixAxisSensorLifoHandle = 65,
        Method_StartSixAxisSensor = 66,
        Method_StopSixAxisSensor = 67,
        Method_IsSixAxisSensorFusionEnabled = 68,
        Method_EnableSixAxisSensorFusion = 69,
        Method_SetSixAxisSensorFusionParameters = 70,
        Method_GetSixAxisSensorFusionParameters = 71,
        Method_ResetSixAxisSensorFusionParameters = 72,
        Method_SetAccelerometerParameters = 73,
        Method_GetAccelerometerParameters = 74,
        Method_ResetAccelerometerParameters = 75,
        Method_SetAccelerometerPlayMode = 76,
        Method_GetAccelerometerPlayMode = 77,
        Method_ResetAccelerometerPlayMode = 78,
        Method_SetGyroscopeZeroDriftMode = 79,
        Method_GetGyroscopeZeroDriftMode = 80,
        Method_ResetGyroscopeZeroDriftMode = 81,
        Method_IsSixAxisSensorAtRest = 82,
        Method_IsFirmwareUpdateAvailableForSixAxisSensor = 83,
        Method_ActivateGesture = 91,
        Method_SetSupportedNpadStyleSet = 100,
        Method_GetSupportedNpadStyleSet = 101,
        Method_SetSupportedNpadIdType = 102,
        Method_ActivateNpad = 103,
        Method_DeactivateNpad = 104,
        Method_AcquireNpadStyleSetUpdateEventHandle = 106,
        Method_DisconnectNpad = 107,
        Method_GetPlayerLedPattern = 108,
        Method_ActivateNpadWithRevision = 109,
        Method_SetNpadJoyHoldType = 120,
        Method_GetNpadJoyHoldType = 121,
        Method_SetNpadJoyAssignmentModeSingleByDefault = 122,
        Method_SetNpadJoyAssignmentModeSingle = 123,
        Method_SetNpadJoyAssignmentModeDual = 124,
        Method_MergeSingleJoyAsDualJoy = 125,
        Method_StartLrAssignmentMode = 126,
        Method_StopLrAssignmentMode = 127,
        Method_SetNpadHandheldActivationMode = 128,
        Method_GetNpadHandheldActivationMode = 129,
        Method_SwapNpadAssignment = 130,
        Method_IsUnintendedHomeButtonInputProtectionEnabled = 131,
        Method_EnableUnintendedHomeButtonInputProtection = 132,
        Method_SetNpadJoyAssignmentModeSingleWithDestination = 133,
        Method_SetNpadAnalogStickUseCenterClamp = 134,
        Method_SetNpadCaptureButtonAssignment = 135,
        Method_ClearNpadCaptureButtonAssignment = 136,
        Method_GetVibrationDeviceInfo = 200,
        Method_SendVibrationValue = 201,
        Method_GetActualVibrationValue = 202,
        Method_CreateActiveVibrationDeviceList = 203,
        Method_PermitVibration = 204,
        Method_IsVibrationPermitted = 205,
        Method_SendVibrationValues = 206,
        Method_SendVibrationGcErmCommand = 207,
        Method_GetActualVibrationGcErmCommand = 208,
        Method_BeginPermitVibrationSession = 209,
        Method_EndPermitVibrationSession = 210,
        Method_IsVibrationDeviceMounted = 211,
        Method_SendVibrationValueInBool = 212,
        Method_ActivateConsoleSixAxisSensor = 300,
        Method_StartConsoleSixAxisSensor = 301,
        Method_StopConsoleSixAxisSensor = 302,
        Method_ActivateSevenSixAxisSensor = 303,
        Method_StartSevenSixAxisSensor = 304,
        Method_StopSevenSixAxisSensor = 305,
        Method_InitializeSevenSixAxisSensor = 306,
        Method_FinalizeSevenSixAxisSensor = 307,
        Method_SetSevenSixAxisSensorFusionStrength = 308,
        Method_GetSevenSixAxisSensorFusionStrength = 309,
        Method_ResetSevenSixAxisSensorTimestamp = 310,
        Method_IsUsbFullKeyControllerEnabled = 400,
        Method_EnableUsbFullKeyController = 401,
        Method_IsUsbFullKeyControllerConnected = 402,
        Method_HasBattery = 403,
        Method_HasLeftRightBattery = 404,
        Method_GetNpadInterfaceType = 405,
        Method_GetNpadLeftRightInterfaceType = 406,
        Method_GetNpadOfHighestBatteryLevelForJoyLeft = 407,
        Method_GetNpadOfHighestBatteryLevelForJoyRight = 408,
        Method_GetPalmaConnectionHandle = 500,
        Method_InitializePalma = 501,
        Method_AcquirePalmaOperationCompleteEvent = 502,
        Method_GetPalmaOperationInfo = 503,
        Method_PlayPalmaActivity = 504,
        Method_SetPalmaFrModeType = 505,
        Method_ReadPalmaStep = 506,
        Method_EnablePalmaStep = 507,
        Method_ResetPalmaStep = 508,
        Method_ReadPalmaApplicationSection = 509,
        Method_WritePalmaApplicationSection = 510,
        Method_ReadPalmaUniqueCode = 511,
        Method_SetPalmaUniqueCodeInvalid = 512,
        Method_WritePalmaActivityEntry = 513,
        Method_WritePalmaRgbLedPatternEntry = 514,
        Method_WritePalmaWaveEntry = 515,
        Method_SetPalmaDataBaseIdentificationVersion = 516,
        Method_GetPalmaDataBaseIdentificationVersion = 517,
        Method_SuspendPalmaFeature = 518,
        Method_GetPalmaOperationResult = 519,
        Method_ReadPalmaPlayLog = 520,
        Method_ResetPalmaPlayLog = 521,
        Method_SetIsPalmaAllConnectable = 522,
        Method_SetIsPalmaPairedConnectable = 523,
        Method_PairPalma = 524,
        Method_SetPalmaBoostMode = 525,
        Method_CancelWritePalmaWaveEntry = 526,
        Method_EnablePalmaBoostMode = 527,
        Method_GetPalmaBluetoothAddress = 528,
        Method_SetDisallowedPalmaConnection = 529,
        Method_SetNpadCommunicationMode = 1000,
        Method_GetNpadCommunicationMode = 1001,
        Method_SetTouchScreenConfiguration = 1002,
        Method_IsFirmwareUpdateNeededForNotification = 1003,
        Method_ActivateDigitizer = 2000,
    };
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    void Close(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "hid"; }

private:
    HID(void);
    HID(const HID&);
    HID& operator=(const HID&);

    HID(CSwitchSystem & System);

    void ProcessCreateAppletResource(CIPCRequest & Request);
    void ProcessActivateDebugPad(CIPCRequest & Request);
    void ProcessActivateNpad(CIPCRequest & Request);
    void ProcessActivateTouchScreen(CIPCRequest & Request);
    void ProcessSetSupportedNpadIdType(CIPCRequest & Request);
    void ProcessSetSupportedNpadStyleSet(CIPCRequest & Request);
};