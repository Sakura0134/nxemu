#pragma once
#include <nxemu-core\Plugins\VideoPlugin.h>
#include <memory>
#include <string>

class CPlugins
{
public:
    CPlugins();
    ~CPlugins();

    bool Initiate(IRenderWindow & RenderWindow, ISwitchSystem & System);
    void CreatePlugins(void);

    IVideo * VideoPlugin(void) { return m_Video; }

private:
    CPlugins(const CPlugins&);
    CPlugins& operator=(const CPlugins&);

    std::unique_ptr<CVideoPlugin> m_VideoPlugin;
    IVideo * m_Video;
    std::string m_VideoFile;
};
