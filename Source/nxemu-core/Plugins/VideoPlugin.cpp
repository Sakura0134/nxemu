#include <nxemu-core\Plugins\VideoPlugin.h>
#include <nxemu-core\SystemGlobals.h>

CVideoPlugin::CVideoPlugin() :
    m_CreateVideo(dummyCreateVideo),
    m_DestroyVideo(dummyDestroyVideo)
{
}

void CVideoPlugin::UnloadPlugin(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

bool CVideoPlugin::LoadFunctions(void)
{
    m_CreateVideo = (tyCreateVideo)DynamicLibrary::GetProc(m_hLib, "CreateVideo");
    m_DestroyVideo = (tyDestroyVideo)DynamicLibrary::GetProc(m_hLib, "DestroyVideo");

    bool res = true;
    if (m_CreateVideo == nullptr)
    {
        m_CreateVideo = dummyCreateVideo;
        res = false;
    }
    if (m_DestroyVideo == nullptr)
    {
        m_DestroyVideo = dummyDestroyVideo;
        res = false;
    }
    return res;
}

IVideo * CVideoPlugin::dummyCreateVideo(IRenderWindow& /*RenderWindow*/, ISwitchSystem & /*System*/)
{
    return nullptr;
}

void CVideoPlugin::dummyDestroyVideo(IVideo * /*Video*/)
{
}
