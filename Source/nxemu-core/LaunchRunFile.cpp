#include "Machine\SwitchRom.h"
#include <string>

bool LaunchRunFile(IRenderWindow & Window, int argc, char **argv)
{
    std::string LaunchFile;

    for (int32_t i = 1; i < argc; i++)
    {
        if (!LaunchFile.empty())
        {
            return false;
        }
        LaunchFile = argv[i];
    }
    if (LaunchFile.length() == 0)
    {
        return true;
    }
    if (!LaunchSwitchRom(Window, LaunchFile.c_str()))
    {
        return false;
    }
    return true;
}