#include <nxemu-core\hle\Services\am\ISelfController.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObjectPtr ISelfController::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new ISelfController(System));
}

ISelfController::ISelfController(CSwitchSystem & System) :
    CService(System)
{
}

bool ISelfController::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode ISelfController::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_SetFocusHandlingMode:
    case Method_SetOutOfFocusSuspendingEnabled:
        break;
    case Method_CreateManagedDisplayLayer:
        return ProcessCreateManagedDisplayLayer(Request);
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}

ResultCode ISelfController::ProcessCreateManagedDisplayLayer(CIPCRequest & Request)
{
    CDisplay & Display = m_System.HleKernel().GetDisplay();
    uint32_t DisplayId = 0;
    if (!Display.Open("Default", DisplayId))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    uint32_t LayerId = 0;
    if (!Display.CreateLayer(DisplayId, LayerId))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }

    CIPCRequest::REQUEST_DATA & ResponseData = Request.ResponseData();
    ResponseData.resize(sizeof(uint32_t) * 2);
    ((uint32_t *)ResponseData.data())[0] = LayerId;
    return RESULT_SUCCESS;
}
