#pragma once
#include <nxemu-core\hle\Display\Nvidia\nvResult.h>
#include <nxemu-core\hle\Display\Nvidia\nvIoctl.h>
#include <nxemu-core\hle\Services\IpcRequest.h>

__interface INvDevice
{
    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData) = 0;
};
