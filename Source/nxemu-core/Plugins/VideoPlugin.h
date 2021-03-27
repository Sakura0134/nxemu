#pragma once
#include <nxemu-core\Plugins\PluginBase.h>
#include <nxemu-plugin-spec\Video.h>

class CVideoPlugin :
    public CPluginBase
{
public:
    typedef IVideo* (CALL* tyCreateVideo)(IRenderWindow& RenderWindow, ISwitchSystem& System);
    typedef void (CALL* tyDestroyVideo)(IVideo* Video);

    CVideoPlugin();

    IVideo* CreateVideo(IRenderWindow & RenderWindow, ISwitchSystem & System) const { return m_CreateVideo(RenderWindow, System); }
    void DestroyVideo(IVideo * Video) const { m_DestroyVideo(Video); }

protected:
    void UnloadPlugin(void);
    bool LoadFunctions(void);
    PLUGIN_TYPE type() const { return PLUGIN_TYPE_VIDEO; }

private:
    CVideoPlugin(const CVideoPlugin&);
    CVideoPlugin& operator=(const CVideoPlugin&);

    static IVideo * CALL dummyCreateVideo(IRenderWindow & RenderWindow, ISwitchSystem & System);
    static void CALL dummyDestroyVideo(IVideo * Video);

    tyCreateVideo m_CreateVideo;
    tyDestroyVideo m_DestroyVideo;
};
