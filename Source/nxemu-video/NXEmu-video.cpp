#include <nxemu-plugin-spec\Video.h>
#include "Video.h"
#include <stdio.h>
#include <Common\Trace.h>
#include <Common\TraceModulesCommon.h>
#include <memory>

std::unique_ptr<CVideo> g_Video;
IPluginNotification * g_Notify = nullptr;

/******************************************************************
  Function: GetPluginInfo
  Purpose:  This function allows the emulator to gather information
  about the dll by filling in the PluginInfo structure.
  input:    a pointer to a PLUGIN_INFO stucture that needs to be
  filled by the function. (see def above)
  output:   none
*******************************************************************/
void CALL GetPluginInfo(PLUGIN_INFO * PluginInfo)
{
    PluginInfo->Version = VIDEO_SPECS_VERSION;
    PluginInfo->Type = PLUGIN_TYPE_VIDEO;  // Set to PLUGIN_TYPE_GFX
#ifdef _DEBUG
    sprintf(PluginInfo->Name, "NXEmu Video Plugin (Debug)");
#else
    sprintf(PluginInfo->Name, "NXEmu Video Plugin");
#endif
}

/******************************************************************
  Function: SetPluginNotification
  Purpose:  This function gives the plugin the ability to notify
            the emulator with messages.
  input:    a pointer to a IPluginNotification interface
  output:   none
*******************************************************************/
void CALL SetPluginNotification(IPluginNotification & Notification)
{
    g_Notify = &Notification;
}

IVideo * CALL CreateVideo(IRenderWindow & RenderWindow, ISwitchSystem & System)
{
    if (g_Notify == nullptr)
    {
        return nullptr;
    }
    if (g_Video.get() != nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }
    TraceSetMaxModule(MaxTraceModuleCommon, TraceError);
    g_Video = std::make_unique<CVideo>(RenderWindow, System);
    return g_Video.get();
}

void CALL DestroyVideo(IVideo * Video)
{
    if (Video == nullptr /*|| g_VideoGpu.get() != Gpu*/)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    g_Video = nullptr;
}

extern "C" int __stdcall DllMain(void * /*hinst*/, unsigned long /*fdwReason*/, void * /*lpReserved*/)
{
    return true;
}
