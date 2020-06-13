#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\hle\Memory\SystemThreadMemory.h>
#include <nxemu-core\hle\Kernel\ResultCode.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <vector>
#include <memory>

__interface IService;
class CSwitchSystem;

class CIPCRequest
{
public:

private:
    CIPCRequest(void);
    CIPCRequest(const CIPCRequest&);
    CIPCRequest& operator=(const CIPCRequest&);
};
