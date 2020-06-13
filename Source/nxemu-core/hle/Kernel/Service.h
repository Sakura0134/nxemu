#pragma once
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <map>
#include <string>

class CSwitchSystem;

class CService :
    public CKernelObject
{
public:
    CService(CSwitchSystem & System);
    virtual ~CService();

    virtual bool Connect(void) = 0;
    virtual const char * Name(void) const = 0;
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
};
