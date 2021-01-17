#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IApplicationDisplayService :
    public CService
{
public:
    enum Method
    {
        Method_GetRelayService = 100,
        Method_GetSystemDisplayService = 101,
        Method_GetManagerDisplayService = 102,
        Method_GetIndirectDisplayTransactionService = 103,
        Method_ListDisplays = 1000,
        Method_OpenDisplay = 1010,
        Method_OpenDefaultDisplay = 1011,
        Method_CloseDisplay = 1020,
        Method_SetDisplayEnabled = 1101,
        Method_GetDisplayResolution = 1102,
        Method_OpenLayer = 2020,
        Method_CloseLayer = 2021,
        Method_CreateStrayLayer = 2030,
        Method_DestroyStrayLayer = 2031,
        Method_SetLayerScalingMode = 2101,
        Method_ConvertScalingMode = 2102,
        Method_GetIndirectLayerImageMap = 2450,
        Method_GetIndirectLayerImageCropMap = 2451,
        Method_GetIndirectLayerImageRequiredMemoryInfo = 2460,
        Method_GetDisplayVsyncEvent = 5202,
        Method_GetDisplayVsyncEventForDebug = 5203,
    };
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IApplicationDisplayService"; }

private:
    IApplicationDisplayService(void);
    IApplicationDisplayService(const IApplicationDisplayService&);
    IApplicationDisplayService& operator=(const IApplicationDisplayService&); 
    
    IApplicationDisplayService(CSwitchSystem & System);

    ResultCode ViOpenDisplay(CIPCRequest & Request);
    ResultCode ViOpenLayer(CIPCRequest & Request);
    void ViSetLayerScalingMode(CIPCRequest & Request);
};