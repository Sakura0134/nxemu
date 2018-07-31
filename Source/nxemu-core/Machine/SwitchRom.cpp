#include <nxemu-core\SystemGlobals.h>
#include <Common\path.h>

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
    g_Notify->BreakPoint(__FILE__, __LINE__);

    return true;
}
