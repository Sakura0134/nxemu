#pragma once
#include <nxemu-core\Debugger.h>

class CDebugCommandsView;
class CPUExecutor;

class CDebuggerUI :
    public CDebugger
{
public:
    CDebuggerUI();
    ~CDebuggerUI();

    void OpenCommandWindow(void);
    void EditLoggingOptions(void * hWndParent);
    void WaitForStep(void);

private:
    CDebuggerUI(const CDebuggerUI&);				// Disable copy constructor
    CDebuggerUI& operator=(const CDebuggerUI&);		// Disable assignment

    void SetDebugThread(void);

    CDebugCommandsView * m_CommandsView;

    CPUExecutor * m_Executor;
};
