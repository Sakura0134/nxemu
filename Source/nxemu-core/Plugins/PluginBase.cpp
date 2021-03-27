#include <nxemu-core\Plugins\PluginBase.h>
#include <nxemu-core\SystemGlobals.h>

CPluginBase::CPluginBase() :
    m_hLib(nullptr),
    m_PluginInfo({0})
{
}

bool CPluginBase::Load(const char* FileName)
{
    if (m_hLib != nullptr)
    {
        UnloadPlugin();
    }

    m_hLib = DynamicLibrary::Open(FileName);
    if (m_hLib == nullptr)
    {
        return false;
    }

    CPluginBase::tyGetPluginInfo GetPluginInfo = (CPluginBase::tyGetPluginInfo)DynamicLibrary::GetProc(m_hLib, "GetPluginInfo");
    if (GetPluginInfo == nullptr) { return false; }

    GetPluginInfo(&m_PluginInfo);
    if (!ValidPluginVersion(m_PluginInfo)) { return false; }
    if (m_PluginInfo.Type != type()) { return false; }

    CPluginBase::tySetPluginNotification SetPluginNotification = (CPluginBase::tySetPluginNotification)DynamicLibrary::GetProc(m_hLib, "SetPluginNotification");
    if (SetPluginNotification != nullptr)
    {
        SetPluginNotification(*this);
    }

    if (!LoadFunctions())
    {
        return false;
    }
    return true;
}

void CPluginBase::UnloadPlugin(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

bool CPluginBase::ValidPluginVersion(PLUGIN_INFO & PluginInfo)
{
    if (PluginInfo.Type == PLUGIN_TYPE_VIDEO && PluginInfo.Version == VIDEO_SPECS_VERSION)
    {
        return true;
    }
    return false;
}

void CPluginBase::DisplayError(const char* Message)
{
    g_Notify->DisplayError(Message);
}

void CPluginBase::DisplayMessage(const char* Message)
{
    g_Notify->DisplayMessage(Message);
}

void CPluginBase::DisplayMessageTime(int DisplayTime, const char* Message)
{
    g_Notify->DisplayMessageTime(DisplayTime, Message);
}

void CPluginBase::BreakPoint(const char* FileName, uint32_t LineNumber)
{
    g_Notify->BreakPoint(FileName, LineNumber);
}
