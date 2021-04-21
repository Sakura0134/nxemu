#include <nxemu-core\hle\Services\hid\IAppletResource.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IAppletResource::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IAppletResource(System));
}

IAppletResource::IAppletResource(CSwitchSystem & System) :
    CService(System),
    m_SharedMem(m_System.HleKernel().InterfaceDevice().GetSharedMemory())
{
}

bool IAppletResource::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

void IAppletResource::Close(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

ResultCode IAppletResource::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_GetSharedMemoryHandle:
        ProcessGetSharedMemoryHandle(Request);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void IAppletResource::ProcessGetSharedMemoryHandle(CIPCRequest & Request)
{
    uint32_t handle = m_System.HleKernel().AddKernelObject(m_SharedMem);
    Request.AddResponseHandlesToCopy(handle);
}

