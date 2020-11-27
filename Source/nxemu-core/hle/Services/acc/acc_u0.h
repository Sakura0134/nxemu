#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class ACC_U0 :
    public CService
{
public:
	enum Method
	{
        Method_GetUserCount = 0,
        Method_GetUserExistence = 1,
        Method_ListAllUsers = 2,
        Method_ListOpenUsers = 3,
        Method_GetLastOpenedUser = 4,
        Method_GetProfile = 5,
        Method_GetProfileDigest = 6,
        Method_IsUserRegistrationRequestPermitted = 50,
        Method_TrySelectUserWithoutInteraction = 51,
        Method_ListOpenContextStoredUsers = 60,
        Method_DebugActivateOpenContextRetention = 99,
        Method_InitializeApplicationInfoV0 = 100,
        Method_GetBaasAccountManagerForApplication = 101,
        Method_AuthenticateApplicationAsync = 102,
        Method_CheckNetworkServiceAvailabilityAsync = 103,
        Method_StoreSaveDataThumbnail = 110,
        Method_ClearSaveDataThumbnail = 111,
        Method_CreateGuestLoginRequest = 120,
        Method_LoadOpenContext = 130,
        Method_ListOpenContextStoredUsers2 = 131,
        Method_InitializeApplicationInfo = 140,
        Method_ListQualifiedUsers = 141,
        Method_IsUserAccountSwitchLocked = 150,
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
	void ProcessGetBaasAccountManagerForApplication(CIPCRequest & Request);
};