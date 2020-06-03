#pragma once
#include <map>
#include <vector>

class CKernelObjectPtr;
class CSystemThread;

class CKernelObject
{
    friend CKernelObjectPtr;

public:
    enum HandleType
    {
        Unknown,
        Thread,
    };

    CKernelObject(void);
    virtual ~CKernelObject();

    virtual HandleType GetHandleType() const = 0;
    virtual CSystemThread * GetSystemThreadPtr(void);

private:
    CKernelObject(const CKernelObject&);				// Disable copy constructor
    CKernelObject& operator=(const CKernelObject&);		// Disable assignment

    uint32_t m_ref;
};

class CKernelObjectPtr
{
public:
    CKernelObjectPtr(CKernelObject * Object);
    CKernelObjectPtr(const CKernelObjectPtr&);
    CKernelObjectPtr& operator=(const CKernelObjectPtr&);
    ~CKernelObjectPtr();

    CKernelObject *get() const;
    CKernelObject *operator->() const;
    
    operator CKernelObject *() { return m_Object; }

private:
    void ReleaseRef();

    CKernelObject * m_Object;
};

typedef std::map<uint32_t, CKernelObjectPtr> KernelObjectMap;
typedef std::vector<CKernelObjectPtr> KernelObjectList;
