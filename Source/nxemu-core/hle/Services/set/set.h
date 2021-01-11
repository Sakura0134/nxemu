#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class SetServices :
    public CService
{
public:
    enum LanguageCode : uint64_t 
    {
        LanguageCode_JA = 0x000000000000616A,
        LanguageCode_EN_US = 0x00000053552D6E65,
        LanguageCode_FR = 0x0000000000007266,
        LanguageCode_DE = 0x0000000000006564,
        LanguageCode_IT = 0x0000000000007469,
        LanguageCode_ES = 0x0000000000007365,
        LanguageCode_ZH_CN = 0x0000004E432D687A,
        LanguageCode_KO = 0x0000000000006F6B,
        LanguageCode_NL = 0x0000000000006C6E,
        LanguageCode_PT = 0x0000000000007470,
        LanguageCode_RU = 0x0000000000007572,
        LanguageCode_ZH_TW = 0x00000057542D687A,
        LanguageCode_EN_GB = 0x00000042472D6E65,
        LanguageCode_FR_CA = 0x00000041432D7266,
        LanguageCode_ES_419 = 0x00003931342D7365,
        LanguageCode_ZH_HANS = 0x00736E61482D687A,
        LanguageCode_ZH_HANT = 0x00746E61482D687A,
    };

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

    typedef std::vector<LanguageCode> LanguageCodes;

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

    void ProcessGetAvailableLanguageCodes(CIPCRequest & Request);

    LanguageCodes m_AvailableLanguage;
};