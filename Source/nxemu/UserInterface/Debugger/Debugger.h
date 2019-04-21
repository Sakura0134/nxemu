#pragma once
#include <nxemu-core\Debugger.h>

class CDebuggerUI :
    public CDebugger
{
public:
    CDebuggerUI();
    ~CDebuggerUI();

    void EditLoggingOptions(void * hWndParent);
    void WaitForStep(void);

private:
    CDebuggerUI(const CDebuggerUI&);				// Disable copy constructor
    CDebuggerUI& operator=(const CDebuggerUI&);		// Disable assignment

};
