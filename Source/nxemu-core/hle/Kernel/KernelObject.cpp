#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\hle\Memory\TransferMemory.h>
#include <nxemu-core\SystemGlobals.h>

CKernelObject::CKernelObject() :
    m_ref(0)
{
}

CKernelObject::~CKernelObject()
{
}

CSystemThread * CKernelObject::GetSystemThreadPtr(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

CService * CKernelObject::GetServicePtr(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

KEvent * CKernelObject::GetKEventPtr(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

CTransferMemory * CKernelObject::GetTransferMemoryPtr(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

CKernelObjectPtr::CKernelObjectPtr() :
    m_Object(nullptr)
{
}

CKernelObjectPtr::CKernelObjectPtr(CKernelObject * Object) :
    m_Object(Object)
{
    if (m_Object)
    {
        m_Object->m_ref += 1;
    }
}

CKernelObjectPtr::CKernelObjectPtr(const CKernelObjectPtr&rhs) :
    m_Object(rhs.m_Object)
{
    if (m_Object)
    {
        m_Object->m_ref += 1;
    }
}

CKernelObjectPtr& CKernelObjectPtr::operator=(const CKernelObjectPtr& rhs)
{
    if (m_Object)
    {
        ReleaseRef();
    }
    m_Object = rhs.m_Object;
    if (m_Object)
    {
        m_Object->m_ref += 1;
    }
    return *this;
}

CKernelObjectPtr::~CKernelObjectPtr()
{
    ReleaseRef();
}

CKernelObject * CKernelObjectPtr::operator->() const
{
    return (get());
}

CKernelObject * CKernelObjectPtr::get() const
{
    return m_Object;
}

void CKernelObjectPtr::reset(CKernelObject* Object)
{
    ReleaseRef();
    m_Object = Object;
    if (m_Object)
    {
        m_Object->m_ref += 1;
    }
}

void CKernelObjectPtr::ReleaseRef()
{
    if (m_Object == nullptr)
    {
        return;
    }
    m_Object->m_ref -= 1;
    if (m_Object->m_ref == 0)
    {
        switch (m_Object->GetHandleType())
        {
        case KernelObjectHandleType_Service:
            delete m_Object->GetServicePtr();
            break;
        case KernelObjectHandleType_TransferMemory:
            delete m_Object->GetTransferMemoryPtr();
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    m_Object = nullptr;
}
