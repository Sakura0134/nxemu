#pragma once
#include <Common\DynamicLibrary.h>

#define EXPORT
#include <nxemu-plugin-spec\Base.h>

class CPluginBase :
    public IPluginNotification
{
    typedef void (CALL * tyGetPluginInfo) (PLUGIN_INFO * PluginInfo);
    typedef void (CALL * tySetPluginNotification) (IPluginNotification & Notification);

public:
    CPluginBase();

    bool Load(const char * FileName);

protected:
    virtual void UnloadPlugin(void);
    virtual bool LoadFunctions(void) = 0;
    virtual PLUGIN_TYPE type() const = 0;

    static bool ValidPluginVersion(PLUGIN_INFO& PluginInfo);

    DynamicLibrary::DynLibHandle m_hLib;
    PLUGIN_INFO m_PluginInfo;

private:
    CPluginBase(const CPluginBase&);
    CPluginBase& operator=(const CPluginBase&);

    //IPluginNotification
    void DisplayError(const char* Message);
    void DisplayMessage(const char* Message);
    void DisplayMessageTime(int DisplayTime, const char* Message);
    void BreakPoint(const char* FileName, uint32_t LineNumber);
};
