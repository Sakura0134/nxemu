#pragma once
#include <Common\SyncEvent.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <nxemu-core\Debugger.h>

class CDebugCommandsView;

class CDebuggerUI :
    public CDebugger
{
public:
    CDebuggerUI();
    ~CDebuggerUI();

    void OpenCommandWindow(void);
    void EditLoggingOptions(void * hWndParent);
    void WaitForStep(void);

    inline CKernelObjectPtr DebugThread() { return m_DebugThread; }

private:
    friend CDebugCommandsView;

    CDebuggerUI(const CDebuggerUI&);
    CDebuggerUI& operator=(const CDebuggerUI&);

    static void SteppingOpsChanged(CDebuggerUI * _this);

    void SetDebugThread(void);

    CDebugCommandsView * m_CommandsView;
    SyncEvent m_StepEvent;

    CKernelObjectPtr m_DebugThread;
};
