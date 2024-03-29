#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <memory>

bool LaunchSwitchRom(IRenderWindow & Window, const char * GamePath)
{
    if (g_BaseMachine != nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
    }

    std::unique_ptr<CSwitchSystem> SwitchSystem(new CSwitchSystem());
    if (SwitchSystem.get() == nullptr)
    {
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
    }
    if (!SwitchSystem->Initialize(Window))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
	if (!SwitchSystem->LoadGame(GamePath))
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}
    g_BaseMachine = SwitchSystem.release();
    g_BaseMachine->StartEmulation();
    return true;
}
