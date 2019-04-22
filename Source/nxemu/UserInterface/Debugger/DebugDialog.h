#pragma once
#include <nxemu\UserInterface\WTLApp.h>

class CDebuggerUI;

extern WTLModule _Module;

template <class T>
class CDebugDialog :
    public CDialogImpl < T >
{
protected:
    CDebuggerUI * m_Debugger;
    HANDLE        m_CreatedEvent;
    HANDLE        m_DialogThread;

    static DWORD CreateDebuggerWindow(CDebugDialog<T> * pThis)
    {
        _Module.AddCreateWndData(&pThis->m_thunk.cd, pThis);
        INT_PTR res = ::DialogBoxParamW(NULL, MAKEINTRESOURCEW(static_cast<T*>(pThis)->IDD), NULL, StartDebugDialogProc, 0);
        if (res != 0)
        {
            pThis->WindowCreated();
        }
        return 0;
    }

    void WindowCreated(void)
    {
        SetEvent(m_CreatedEvent);
    }

public:
    CDebugDialog(CDebuggerUI * debugger) :
        m_Debugger(debugger),
        m_CreatedEvent(CreateEvent(NULL, true, false, NULL)),
        m_DialogThread(NULL)
    {
    }
    virtual ~CDebugDialog(void)
    {
        HideWindow();
        CloseHandle(m_CreatedEvent);
        if (m_DialogThread)
        {
            CloseHandle(m_DialogThread);
            m_DialogThread = NULL;
        }
    }

    void HideWindow(void)
    {
        if (m_hWnd && ::IsWindow(m_hWnd))
        {
            ::EndDialog(m_hWnd, 0);
        }
        if (m_DialogThread)
        {
            if (WaitForSingleObject(m_DialogThread, 5000) == WAIT_TIMEOUT)
            {
                TerminateThread(m_DialogThread, 1);
            }
            CloseHandle(m_DialogThread);
            m_DialogThread = NULL;
        }
    }

    void ShowWindow(void)
    {
        if (m_hWnd)
        {
            SetForegroundWindow((HWND)m_hWnd);
        }
        else
        {
            DWORD ThreadID;
            ResetEvent(m_CreatedEvent);
            m_DialogThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CreateDebuggerWindow, (LPVOID)this, 0, &ThreadID);
            WaitForSingleObject(m_CreatedEvent, 20000);
        }
    }

private:
    static INT_PTR CALLBACK StartDebugDialogProc(
        _In_ HWND hWnd,
        _In_ UINT uMsg,
        _In_ WPARAM wParam,
        _In_ LPARAM lParam)
    {
        CDebugDialog<T> * pThis = (CDebugDialog<T> *)_Module.ExtractCreateWndData();
        ATLASSERT(pThis != NULL);
        if (!pThis)
        {
            return 0;
        }
        pThis->m_hWnd = hWnd;
#ifdef _DEBUG
        pThis->m_bModal = TRUE;
#endif
        // Initialize the thunk.  This was allocated in CDialogImpl::DoModal or
        // CDialogImpl::Create, so failure is unexpected here.

        pThis->m_thunk.Init((WNDPROC)pThis->GetDialogProc(), pThis);
        DLGPROC pProc = (DLGPROC)pThis->m_thunk.GetWNDPROC();
        DLGPROC pOldProc = (DLGPROC)::SetWindowLongPtrW(hWnd, DWLP_DLGPROC, (LONG_PTR)pProc);
#ifdef _DEBUG
        // check if somebody has subclassed us already since we discard it
        if (pOldProc != StartDialogProc)
            ATLTRACE(atlTraceWindowing, 0, _T("Subclassing through a hook discarded.\n"));
#else
        DBG_UNREFERENCED_LOCAL_VARIABLE(pOldProc);	// avoid unused warning
#endif
        return pProc(hWnd, uMsg, wParam, lParam);
    }

};
