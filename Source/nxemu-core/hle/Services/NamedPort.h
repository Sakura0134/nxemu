#pragma once
#include <map>
#include <string>
#include <vector>
#include <nxemu-core\hle\Services\IpcRequest.h>

__interface NamedPortInstance
{
    virtual bool connect(void) = 0;
    virtual uint64_t CallMethod(CIPCRequest & Request) = 0;
    virtual const char * name(void) const = 0;
};

