#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class ACC_U0 :
    public CService
{
public:
	enum Method
	{
        GetUserCount = 0,
        GetUserExistence = 1,
        ListAllUsers = 2,
        ListOpenUsers = 3,
        GetLastOpenedUser = 4,
        GetProfile = 5,
        GetProfileDigest = 6,
        IsUserRegistrationRequestPermitted = 50,
        TrySelectUserWithoutInteraction = 51,
        ListOpenContextStoredUsers = 60,
        DebugActivateOpenContextRetention = 99,
        InitializeApplicationInfoV0 = 100,
        GetBaasAccountManagerForApplication = 101,
        AuthenticateApplicationAsync = 102,
        CheckNetworkServiceAvailabilityAsync = 103,
        StoreSaveDataThumbnail = 110,
        ClearSaveDataThumbnail = 111,
        CreateGuestLoginRequest = 120,
        LoadOpenContext = 130,
        ListOpenContextStoredUsers2 = 131,
        InitializeApplicationInfo = 140,
        ListQualifiedUsers = 141,
        IsUserAccountSwitchLocked = 150,
	};
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "acc:u0"; }

private:
	ACC_U0(void);
	ACC_U0(const ACC_U0&);
	ACC_U0& operator=(const ACC_U0&);

    ACC_U0(CSwitchSystem & System);

	void ProcessInitializeApplicationInfo(void);
};