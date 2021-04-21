#include <nxemu-core\hle\Services\hid\hid.h>
#include <nxemu-core\hle\Services\hid\IAppletResource.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr HID::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new HID(System));
}

HID::HID(CSwitchSystem & System) :
    CService(System)
{
}

bool HID::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

void HID::Close(void)
{
    delete this;
}

ResultCode HID::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_CreateAppletResource: ProcessCreateAppletResource(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void HID::ProcessCreateAppletResource(CIPCRequest & Request)
{
    Request.MakeObject(IAppletResource::CreateInstance(m_System)->GetServicePtr());
}
