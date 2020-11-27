#include <nxemu\UserInterface\Debugger\Debugger.h>
#include <nxemu\UserInterface\Debugger\Debugger-Commands.h>
#include <nxemu-core\Settings\Settings.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Machine\SwitchSystem.h>

CDebuggerUI::CDebuggerUI() :
    m_CommandsView(nullptr),
    m_StepEvent(false),
    m_DebugThread(nullptr)
{
	g_Settings->RegisterChangeCB(Debugger_SteppingOps, this, (CSettings::SettingChangedFunc)SteppingOpsChanged);
}

CDebuggerUI::~CDebuggerUI()
{
    if (g_Settings)
    {
        g_Settings->UnregisterChangeCB(Debugger_SteppingOps, this, (CSettings::SettingChangedFunc)SteppingOpsChanged);
    }
    if (m_CommandsView != nullptr)
    {
        delete m_CommandsView;
        m_CommandsView = nullptr;
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
    if (m_CommandsView == nullptr)
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
    const KernelObjectMap & KernelObjects = g_BaseMachine != nullptr ? g_BaseMachine->KernelObjects() : KernelObjectMap();
    if (m_DebugThread != nullptr)
    {
        bool Found = false;
        for (KernelObjectMap::const_iterator itr = KernelObjects.begin(); itr != KernelObjects.end(); itr++)
        {
            CKernelObject * Object = itr->second.get();
            if (Object->GetHandleType() != CKernelObject::Thread)
            {
                continue;
            }
            CSystemThread * Thread = Object->GetSystemThreadPtr();
            if (m_DebugThread == Thread)
            {
                Found = true;
                break;
            }
        }
        if (!Found)
        {
            m_DebugThread = nullptr;
        }
    }

    if (m_DebugThread == nullptr)
    {
        CKernelObjectPtr FirstThread(nullptr);
        for (KernelObjectMap::const_iterator itr = KernelObjects.begin(); itr != KernelObjects.end(); itr++)
        {
            CKernelObject * Object = itr->second.get();
            if (Object->GetHandleType() != CKernelObject::Thread)
            {
                continue;
            }
            CSystemThread * Thread = Object->GetSystemThreadPtr();
            if (FirstThread == nullptr)
            {
                FirstThread = Thread;
            }
            if (Thread->GetState() == CSystemThread::Running)
            {
                m_DebugThread = Object;
                break;
            }
        }
        if (m_DebugThread == nullptr)
        {
            m_DebugThread = FirstThread;
            if (m_DebugThread == nullptr)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
    }
}
