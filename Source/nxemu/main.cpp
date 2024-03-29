#include "UserInterface/Notification.h"
#include <Common/path.h>
#include <Common/StdString.h>
#include "UserInterface/MainWindow.h"
#include <nxemu-core/AppInit.h>
#include <nxemu-core/SystemGlobals.h>
#include <nxemu-core/Version.h>
#include <memory>
#include <windows.h>

bool LaunchRunFile(IRenderWindow & Window, int argc, char **argv);

WTLModule _Module;

int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpszArgs*/, int /*nWinMode*/)
{
    AppInit(&Notify(), CPath(CPath::MODULE_DIRECTORY));
    std::unique_ptr<CMainGui> MainWindow(new CMainGui(stdstr_f("NXEmu %s", VER_FILE_VERSION_STR).ToUTF16().c_str()));
    g_Debugger = MainWindow.get();
    Notify().SetMainWindow(MainWindow.get());

    if (!LaunchRunFile(*MainWindow.get(),__argc, __argv))
    {
        AppCleanup();
        return false;
    }
    MainWindow->Show(true);	//Show the main window
    MainWindow->ProcessAllMessages();
    MainWindow.reset(nullptr);
    AppCleanup();
    return true;
}