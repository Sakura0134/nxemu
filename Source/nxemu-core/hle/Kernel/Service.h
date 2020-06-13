#pragma once
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <map>
#include <string>

typedef std::map<std::string, CKernelObjectPtr> NamedPortList;

class CService :
    public CKernelObject
{
public:
    CService(CSwitchSystem & System);
    virtual ~CService();

    virtual bool Connect(void) = 0;

private:
    CService(void);
    CService(const CService&);
    CService& operator=(const CService&);
};
