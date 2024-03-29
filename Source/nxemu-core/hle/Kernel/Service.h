#pragma once
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <nxemu-core\hle\Kernel\ResultCode.h>
#include <nxemu-core\hle\Services\IpcRequest.h>
#include <map>
#include <string>
#include <memory>

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
    bool FindDomainService(int32_t ObjectID, CService *& Service);
    int32_t AddDomainObject(CService * Service);

	int32_t ConvertToDomain(void);

    inline bool IsDomain(void) const { return m_Domain;  }

protected:
    CSwitchSystem & m_System;

private:
    CService(void);
    CService(const CService&);
    CService& operator=(const CService&);
	
    KernelObjectHandleType GetHandleType() const { return KernelObjectHandleType_Service; }
    CService * GetServicePtr(void) { return this; }

	bool m_Domain;
	int32_t m_ObjectId;
	uint32_t m_NextDomanObjectId;
	IServiceList m_DomainObjects;
};
