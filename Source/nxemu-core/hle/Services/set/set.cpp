#include <nxemu-core\hle\Services\ServiceManager.h>
#include <nxemu-core\hle\Services\set\Set.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr SetServices::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new SetServices(System));
}

SetServices::SetServices(CSwitchSystem & System) :
    CService(System)
{
    m_AvailableLanguage.push_back(LanguageCode_JA);
    m_AvailableLanguage.push_back(LanguageCode_EN_US);
    m_AvailableLanguage.push_back(LanguageCode_FR);
    m_AvailableLanguage.push_back(LanguageCode_DE);
    m_AvailableLanguage.push_back(LanguageCode_IT);
    m_AvailableLanguage.push_back(LanguageCode_ES);
    m_AvailableLanguage.push_back(LanguageCode_ZH_CN);
    m_AvailableLanguage.push_back(LanguageCode_KO);
    m_AvailableLanguage.push_back(LanguageCode_NL);
    m_AvailableLanguage.push_back(LanguageCode_PT);
    m_AvailableLanguage.push_back(LanguageCode_RU);
    m_AvailableLanguage.push_back(LanguageCode_ZH_TW);
    m_AvailableLanguage.push_back(LanguageCode_EN_GB);
    m_AvailableLanguage.push_back(LanguageCode_FR_CA);
    m_AvailableLanguage.push_back(LanguageCode_ES_419);
    m_AvailableLanguage.push_back(LanguageCode_ZH_HANS);
    m_AvailableLanguage.push_back(LanguageCode_ZH_HANT);
}

bool SetServices::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode SetServices::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_Method_GetAvailableLanguageCodes:
        ProcessGetAvailableLanguageCodes(Request);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void SetServices::ProcessGetAvailableLanguageCodes(CIPCRequest & Request)
{
    uint32_t MaxEntries = 0xF; //pre 4.0 is 0xF post 4.0 is 0x40
    uint32_t AvailableLanguage = (uint32_t)(m_AvailableLanguage.size() > MaxEntries ? MaxEntries : m_AvailableLanguage.size());
    Request.WriteBuffer((const uint8_t *)m_AvailableLanguage.data(), AvailableLanguage * sizeof(uint64_t));
    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t));
    ((uint32_t *)ResponseData.data())[0] = AvailableLanguage;
}
