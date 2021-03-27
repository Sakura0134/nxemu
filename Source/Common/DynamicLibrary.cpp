#include "stdafx.h"
#include "DynamicLibrary.h"
#include "StdString.h"
#ifdef _WIN32
#include <windows.h>
#endif

DynamicLibrary::DynLibHandle DynamicLibrary::Open(const char *pccLibraryPath, bool ShowErrors)
{
    if (pccLibraryPath == nullptr)
    {
        return nullptr;
    }
#ifdef _WIN32
    UINT LastErrorMode = SetErrorMode(ShowErrors ? 0 : SEM_FAILCRITICALERRORS);
    SetErrorMode(0);
    DynLibHandle lib = (DynLibHandle)LoadLibraryA(pccLibraryPath);
    SetErrorMode(LastErrorMode);
#else
    DynLibHandle lib = nullptr;
#endif
    return lib;
}

void DynamicLibrary::Close(DynLibHandle LibHandle)
{
    if (LibHandle != nullptr)
    {
#ifdef _WIN32
        FreeLibrary((HMODULE)LibHandle);
#else
        dlclose(LibHandle);
#endif
    }
}

void * DynamicLibrary::GetProc(DynLibHandle LibHandle, const char * ProcedureName)
{
    if (ProcedureName == nullptr)
    {
        return nullptr;
    }

#ifdef _WIN32
    return GetProcAddress((HMODULE)LibHandle, ProcedureName);
#else
    return nullptr;
#endif
}

std::string DynamicLibrary::GetVersionedFilename(const char* LibName, int major, int minor)
{
#if defined(_WIN32)
    if (major >= 0 && minor >= 0)
    {
        return stdstr_f("%s-%d-%d.dll", LibName, major, minor);
    }
    else if (major >= 0)
    {
        return stdstr_f("%s-%d.dll", LibName, major);
    }
    return stdstr_f("%s.dll", LibName);
#else
    return LibName;
#endif
}

