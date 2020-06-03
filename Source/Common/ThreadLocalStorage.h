#pragma once

class CTLSBase
{
public:
    CTLSBase();
    ~CTLSBase();

    void * GetValue(void);
    void SetValue(void * value);

private:
    uint32_t m_tls_key;
};

template < class T >
class CTLS :
    private CTLSBase
{
public:
    CTLS() { }

    T * Get() { return (T *)GetValue(); }
    void Set(T * value) { SetValue((void *)value); }
};
