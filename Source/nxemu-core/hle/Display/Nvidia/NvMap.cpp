#include <nxemu-core\hle\Display\Nvidia\NvMap.h>
#include <nxemu-core\SystemGlobals.h>

CNvMap::CNvMap(void)
{
}

CNvMap::~CNvMap()
{
}

nvResult CNvMap::Ioctl(nvIoctl /*Ioctl*/, const CIPCRequest::RequestBuffer & /*InData*/, CIPCRequest::RequestBuffer & /*OutData*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nvResult_Success;
}