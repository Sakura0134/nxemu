#include <nxemu-core\hle\Services\hid\hid.h>
#include <nxemu-core\hle\Services\hid\IActiveVibrationDeviceList.h>
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
    case Method_ActivateDebugPad: ProcessActivateDebugPad(Request); break;
    case Method_ActivateTouchScreen: ProcessActivateTouchScreen(Request); break;
    case Method_SetSupportedNpadStyleSet: ProcessSetSupportedNpadStyleSet(Request); break;
    case Method_SetSupportedNpadIdType: ProcessSetSupportedNpadIdType(Request); break;
    case Method_ActivateNpad: ProcessActivateNpad(Request); break;
    case Method_CreateActiveVibrationDeviceList: ProcessCreateActiveVibrationDeviceList(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void HID::ProcessCreateActiveVibrationDeviceList(CIPCRequest & Request)
{
    Request.MakeObject(IActiveVibrationDeviceList::CreateInstance(m_System)->GetServicePtr());
}

void HID::ProcessCreateAppletResource(CIPCRequest & Request)
{
    Request.MakeObject(IAppletResource::CreateInstance(m_System)->GetServicePtr());
}

void HID::ProcessActivateDebugPad(CIPCRequest & /*Request*/)
{
    //stub
}

void HID::ProcessActivateNpad(CIPCRequest & /*Request*/)
{
    //stub
}

void HID::ProcessActivateTouchScreen(CIPCRequest & /*Request*/)
{
    //stub
}

void HID::ProcessSetSupportedNpadIdType(CIPCRequest & /*Request*/)
{
    //stub
}

void HID::ProcessSetSupportedNpadStyleSet(CIPCRequest & /*Request*/)
{
    //stub
}
