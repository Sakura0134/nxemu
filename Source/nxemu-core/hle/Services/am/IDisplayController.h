#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IDisplayController :
    public CService
{
public:
    enum Method
    {
        GetLastForegroundCaptureImage = 0,
        UpdateLastForegroundCaptureImage = 1,
        GetLastApplicationCaptureImage = 2,
        GetCallerAppletCaptureImage = 3,
        UpdateCallerAppletCaptureImage = 4,
        GetLastForegroundCaptureImageEx = 5,
        GetLastApplicationCaptureImageEx = 6,
        GetCallerAppletCaptureImageEx = 7,
        TakeScreenShotOfOwnLayer = 8,
        CopyBetweenCaptureBuffers = 9,
        AcquireLastApplicationCaptureBuffer = 10,
        ReleaseLastApplicationCaptureBuffer = 11,
        AcquireLastForegroundCaptureBuffer = 12,
        ReleaseLastForegroundCaptureBuffer = 13,
        AcquireCallerAppletCaptureBuffer = 14,
        ReleaseCallerAppletCaptureBuffer = 15,
        AcquireLastApplicationCaptureBufferEx = 16,
        AcquireLastForegroundCaptureBufferEx = 17,
        AcquireCallerAppletCaptureBufferEx = 18,
        ClearCaptureBuffer = 20,
        ClearAppletTransitionBuffer = 21,
        AcquireLastApplicationCaptureSharedBuffer = 22,
        ReleaseLastApplicationCaptureSharedBuffer = 23,
        AcquireLastForegroundCaptureSharedBuffer = 24,
        ReleaseLastForegroundCaptureSharedBuffer = 25,
        AcquireCallerAppletCaptureSharedBuffer = 26,
        ReleaseCallerAppletCaptureSharedBuffer = 27,
        TakeScreenShotOfOwnLayerEx = 28,
    };

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IDisplayController"; }

private:
    IDisplayController(void);
    IDisplayController(const IDisplayController&);
    IDisplayController& operator=(const IDisplayController&);

    IDisplayController(CSwitchSystem & System);
};
