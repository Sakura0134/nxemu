#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <Common\path.h>
#include <memory>

bool IsNsoFile(const CPath & NsoFile);

bool IsGameDir(const char * Directory)
{
    CPath GameDir(Directory,"");
    if (!GameDir.DirectoryExists() || !GameDir.IsDirectory())
    {
        return false;
    }
    if (!IsNsoFile(CPath(Directory, "main"))) { return false; }
    if (!IsNsoFile(CPath(Directory, "rtld"))) { return false; }
    if (!IsNsoFile(CPath(Directory, "sdk"))) { return false; }

    CPath SubSdk(Directory, "subsdk*");
    for (bool FoundFile = SubSdk.FindFirst(); FoundFile; FoundFile = SubSdk.FindNext())
    {
        if (!IsNsoFile(SubSdk)) { return false; }
    }
    return true;
}

bool LaunchSwitchRom(const char * SwitchFile)
{
    bool GameDir = IsGameDir(SwitchFile);

    if (!GameDir)
    {
        return false;
    }
    if (g_BaseMachine != NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    std::auto_ptr<CSwitchSystem> SwitchSystem(new CSwitchSystem());
    if (SwitchSystem.get() == NULL)
    {
        return false;
    }
    if (GameDir && !SwitchSystem->LoadGameDir(SwitchFile))
    {
        return false;
    }
    if (!SwitchSystem->Initialize())
    {
        return false;
    }
    g_BaseMachine = SwitchSystem.release();
    g_BaseMachine->StartEmulation();
    return true;
}
