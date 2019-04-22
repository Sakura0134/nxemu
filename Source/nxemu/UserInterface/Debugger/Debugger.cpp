#include <nxemu\UserInterface\Debugger\Debugger.h>
#include <nxemu\UserInterface\Debugger\Debugger-Commands.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>

CDebuggerUI::CDebuggerUI() :
    m_CommandsView(NULL),
    m_Executor(NULL)
{
}

CDebuggerUI::~CDebuggerUI()
{
}

void CDebuggerUI::OpenCommandWindow(void)
{
    if (m_CommandsView == NULL)
    {
        SetDebugThread();
        m_CommandsView = new CDebugCommandsView(this);
    }
    m_CommandsView->ShowWindow();
}

void CDebuggerUI::WaitForStep(void)
{
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
