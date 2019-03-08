#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <memory>

bool LaunchSwitchRom(const char * GamePath)
{
    if (g_BaseMachine != NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
    }

    std::auto_ptr<CSwitchSystem> SwitchSystem(new CSwitchSystem());
    if (SwitchSystem.get() == NULL)
    {
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
    }

	if (SwitchSystem->LoadGame(GamePath))
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}
    g_BaseMachine = SwitchSystem.release();
    g_BaseMachine->StartEmulation();
    return true;
}
