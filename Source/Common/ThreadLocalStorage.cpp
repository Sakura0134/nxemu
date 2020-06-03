#include "stdafx.h"
#include "ThreadLocalStorage.h"
#include <Windows.h>

CTLSBase::CTLSBase() :
    m_tls_key(TlsAlloc())
{
}

CTLSBase::~CTLSBase()
{
    TlsFree(m_tls_key);
}

void * CTLSBase::GetValue(void)
{
    return TlsGetValue(m_tls_key);
}

void CTLSBase::SetValue(void * value)
{
    TlsSetValue(m_tls_key, value);
}
