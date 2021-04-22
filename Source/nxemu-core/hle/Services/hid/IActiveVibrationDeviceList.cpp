#include <nxemu-core\hle\Services\hid\IActiveVibrationDeviceList.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr IActiveVibrationDeviceList::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IActiveVibrationDeviceList(System));
}

IActiveVibrationDeviceList::IActiveVibrationDeviceList(CSwitchSystem & System) :
    CService(System)
{
}

bool IActiveVibrationDeviceList::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

void IActiveVibrationDeviceList::Close(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

ResultCode IActiveVibrationDeviceList::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_ActivateVibrationDevice: ProcessActivateVibrationDevice(Request); break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

void IActiveVibrationDeviceList::ProcessActivateVibrationDevice(CIPCRequest & /*Request*/)
{
    //stub
}
