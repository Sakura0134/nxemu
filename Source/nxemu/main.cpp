#include "UserInterface/Notification.h"
#include <Common/path.h>
#include <Common/StdString.h>
#include "UserInterface/MainWindow.h"
#include <nxemu-core/AppInit.h>
#include <nxemu-core/Version.h>
#include <windows.h>

WTLModule _Module;

int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpszArgs*/, int /*nWinMode*/)
{
    AppInit(&Notify(), CPath(CPath::MODULE_DIRECTORY), __argc, __argv);
    CMainGui MainWindow(stdstr_f("NXEmu %s", VER_FILE_VERSION_STR).ToUTF16().c_str());
    MainWindow.Show(true);	//Show the main window
    MainWindow.ProcessAllMessages();

    AppCleanup();
    return true;
}