#pragma once
#include <map>
#include <vector>

class CKernelObjectPtr;
class CSystemThread;
class CService;
class KEvent;
class CTransferMemory;
class CKernelSharedMemory;

enum KernelObjectHandleType
{
    KernelObjectHandleType_Unknown,
    KernelObjectHandleType_Thread,
    KernelObjectHandleType_Service,
    KernelObjectHandleType_Event,
    KernelObjectHandleType_TransferMemory,
    KernelObjectHandleType_SharedMemory,
};

class CKernelObject
{
    friend CKernelObjectPtr;

public:
    CKernelObject(void);
    virtual ~CKernelObject();

    virtual KernelObjectHandleType GetHandleType() const = 0;

    virtual CSystemThread * GetSystemThreadPtr(void);
    virtual CService * GetServicePtr(void);
    virtual KEvent * GetKEventPtr(void);
    virtual CTransferMemory * GetTransferMemoryPtr(void);
    virtual CKernelSharedMemory * GetSharedMemoryPtr(void);

private:
    CKernelObject(const CKernelObject&);
    CKernelObject& operator=(const CKernelObject&);

    uint32_t m_ref;
};

class CKernelObjectPtr
{
public:
    CKernelObjectPtr();
    CKernelObjectPtr(CKernelObject * Object);
    CKernelObjectPtr(const CKernelObjectPtr&);
    CKernelObjectPtr& operator=(const CKernelObjectPtr&);
    ~CKernelObjectPtr();

    CKernelObject *get() const;
    CKernelObject *operator->() const;
    void reset(CKernelObject * Object);
    
    operator CKernelObject *() { return m_Object; }

private:
    void ReleaseRef();

    CKernelObject * m_Object;
};

typedef std::map<uint32_t, CKernelObjectPtr> KernelObjectMap;
typedef std::vector<CKernelObjectPtr> KernelObjectList;
