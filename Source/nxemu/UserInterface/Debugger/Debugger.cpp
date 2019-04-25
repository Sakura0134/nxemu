#include <nxemu\UserInterface\Debugger\Debugger.h>
#include <nxemu\UserInterface\Debugger\Debugger-Commands.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>

CDebuggerUI::CDebuggerUI() :
    m_CommandsView(NULL),
    m_StepEvent(false),
    m_Executor(NULL)
{
	g_Settings->RegisterChangeCB(Debugger_SteppingOps, this, (CSettings::SettingChangedFunc)SteppingOpsChanged);
}

CDebuggerUI::~CDebuggerUI()
{
    if (g_Settings)
    {
        g_Settings->UnregisterChangeCB(Debugger_SteppingOps, this, (CSettings::SettingChangedFunc)SteppingOpsChanged);
    }
    if (m_CommandsView != NULL)
    {
        delete m_CommandsView;
        m_CommandsView = NULL;
    }
}

void CDebuggerUI::SteppingOpsChanged(CDebuggerUI * _this)
{
    bool Stepping = g_Settings->LoadBool(Debugger_SteppingOps);
    if (Stepping)
    {
        _this->OpenCommandWindow();
    }
}

void CDebuggerUI::OpenCommandWindow(void)
{
    if (m_CommandsView == NULL)
    {
        SetDebugThread();
        m_CommandsView = new CDebugCommandsView(this, m_StepEvent);
    }
    m_CommandsView->ShowWindow();
}

void CDebuggerUI::WaitForStep(void)
{
    g_Settings->SaveBool(Debugger_WaitingForStep, true);
    m_StepEvent.IsTriggered(SyncEvent::INFINITE_TIMEOUT);
    g_Settings->SaveBool(Debugger_WaitingForStep, false);
}

void CDebuggerUI::SetDebugThread(void)
{
    const SystemThreadList & threads = g_BaseMachine != NULL ? g_BaseMachine->SystemThreads() : SystemThreadList();
    if (m_Executor == NULL)
    {
        m_Executor = threads.size() > 0 ? threads.begin()->second : NULL;
    }
    else if (threads.size() == 0)
    {
        m_Executor = NULL;
    }
}
