#include <nxemu-core\hle\Services\hid\hid.h>
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

ResultCode HID::CallMethod(CIPCRequest & /*Request*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}
