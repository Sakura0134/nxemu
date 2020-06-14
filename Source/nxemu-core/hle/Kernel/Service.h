#pragma once
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <nxemu-core\hle\Services\IpcRequest.h>
#include <map>
#include <string>

class CSwitchSystem;

class CService :
    public CKernelObject
{
public:
    typedef std::map<uint32_t, CKernelObjectPtr> IServiceList;

    CService(CSwitchSystem & System);
    virtual ~CService();

    virtual bool Connect(void) = 0;
    virtual ResultCode CallMethod(CIPCRequest & Request) = 0;
    virtual const char * Name(void) const = 0;
    int32_t AddDomainObject(CService * Service);
    inline bool IsDomain(void) const { return m_Domain;  }

protected:
    CSwitchSystem & m_System;

private:
    CService(void);
    CService(const CService&);
    CService& operator=(const CService&);
	
    HandleType GetHandleType() const { return Service; }
    CService * GetServicePtr(void) { return this; }

	bool m_Domain;
	uint32_t m_NextDomanObjectId;
	IServiceList m_DomainObjects;
};
