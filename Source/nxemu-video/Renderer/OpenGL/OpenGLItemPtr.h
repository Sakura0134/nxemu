#pragma once

template <typename ItemType>
class OpenGLItemPtr 
{
public:
    OpenGLItemPtr(void) : m_Object(nullptr) {}
    OpenGLItemPtr(ItemType * Object) : m_Object(Object) { AddRef(); }
    OpenGLItemPtr(const OpenGLItemPtr & rhs)
    {
        m_Object = rhs.m_Object;
        AddRef();
    }
    OpenGLItemPtr& operator=(const OpenGLItemPtr & rhs) 
    {
        if (m_Object != rhs.m_Object) 
        {
            ReleaseRef();
            m_Object = rhs.m_Object;
            AddRef();
        }
        return *this;
    }
    ~OpenGLItemPtr() { ReleaseRef(); }

    ItemType * Get() const { return m_Object; }
    ItemType * operator->() const { return m_Object; }
    operator ItemType&() const { return *m_Object; }

    void Reset(ItemType* Object) 
    {
        if (m_Object != Object)
        {
            ReleaseRef();
            m_Object = Object;
            AddRef();
        }
    }

    bool operator!=(const ItemType * rhs) const { return m_Object != rhs; }
    bool operator==(const ItemType * rhs) const { return m_Object == rhs; }
    bool operator!=(const OpenGLItemPtr & rhs) const { return m_Object != rhs.m_Object; }
    bool operator==(const OpenGLItemPtr & rhs) const  { return m_Object == rhs.m_Object; }

private:
    void AddRef(void)
    {
        if (m_Object == nullptr)
        {
            return;
        }
        m_Object->m_Ref += 1;
    }
    void ReleaseRef(void) 
    {
        if (m_Object == nullptr)
        {
            return;
        }
        m_Object->m_Ref -= 1;
        if (m_Object->m_Ref == 0)
        {
            delete m_Object;
            m_Object = nullptr;
        }
    }


    ItemType * m_Object;
};
