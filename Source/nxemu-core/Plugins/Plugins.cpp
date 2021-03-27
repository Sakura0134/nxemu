#include <nxemu-core\Plugins\Plugins.h>
#include <nxemu-core\Settings\SettingsID.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\SystemGlobals.h>
#include <Common\path.h>

template <typename plugin_type> static void LoadPlugin(SettingID PluginSettingID, std::unique_ptr<plugin_type> & plugin, std::string& FileName);

CPlugins::CPlugins() :
    m_Video(nullptr)
{
    CreatePlugins();
}

CPlugins::~CPlugins()
{
    if (m_Video != nullptr)
    {
        m_VideoPlugin->DestroyVideo(m_Video);
        m_Video = nullptr;
    }
}

bool CPlugins::Initiate(IRenderWindow & RenderWindow, ISwitchSystem & System)
{
    if (m_VideoPlugin.get() == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    m_Video = m_VideoPlugin->CreateVideo(RenderWindow, System);
    if (m_Video == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    if (!m_Video->Initialize())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    return true;
}

void CPlugins::CreatePlugins(void)
{
    LoadPlugin(Plugin_Video_Selected, m_VideoPlugin, m_VideoFile);
}

template <typename plugin_type> 
void LoadPlugin(SettingID PluginSettingID, std::unique_ptr<plugin_type>& plugin, std::string & FileName)
{
    CPath PluginDir(g_Settings->LoadStringVal(Directory_Plugin), "");
    FileName = g_Settings->LoadStringVal(PluginSettingID);
    CPath PluginFileName((const char *)PluginDir, FileName.c_str());
    plugin = std::make_unique<plugin_type>();
    if (plugin.get() != nullptr)
    {
        if (!plugin->Load(PluginFileName))
        {
            plugin = nullptr;
        }
    }
}
