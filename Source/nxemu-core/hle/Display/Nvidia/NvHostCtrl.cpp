#include <nxemu-core\hle\Display\Nvidia\NvHostCtrl.h>
#include <SystemGlobals.h>

CNvHostCtrl::CNvHostCtrl(CNvDriver& NvDriver) :
    m_NvDriver(NvDriver)
{
}

nvResult CNvHostCtrl::Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nvResult_Success;
}
