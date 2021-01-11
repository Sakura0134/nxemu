#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class SetServices :
    public CService
{
public:
    enum Method
    {
        Method_GetLanguageCode = 0,
        Method_Method_GetAvailableLanguageCodes = 1,
        Method_MakeLanguageCode = 2,
        Method_GetAvailableLanguageCodeCount = 3,
        Method_GetRegionCode = 4,
        Method_GetAvailableLanguageCodes2 = 5,
        Method_GetAvailableLanguageCodeCount2 = 6,
        Method_GetKeyCodeMap = 7,
        Method_GetQuestFlag = 8,
        Method_GetKeyCodeMap2 = 9,
        Method_GetFirmwareVersionForDebug = 10,
        Method_GetDeviceNickName = 11,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "set"; }

private:
    SetServices(void);
    SetServices(const SetServices&);
    SetServices& operator=(const SetServices&);

    SetServices(CSwitchSystem & System);
};