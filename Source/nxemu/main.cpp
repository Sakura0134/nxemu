#include "stdafx.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpszArgs*/, int /*nWinMode*/)
{
    CMainGui MainWindow(stdstr_f("NXEmu %s", VER_FILE_VERSION_STR).ToUTF16().c_str());
    MainWindow.Show(true);	//Show the main window
    MainWindow.ProcessAllMessages();
    return true;
}