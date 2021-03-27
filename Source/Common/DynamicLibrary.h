#pragma once
#include <string>

class DynamicLibrary
{
public:
    typedef void * DynLibHandle;

    static DynLibHandle Open(const char *pccLibraryPath, bool ShowErrors = true);
    static void Close(DynLibHandle LibHandle);
    static void * GetProc(DynLibHandle LibHandle, const char * ProcedureName);
    static std::string GetVersionedFilename(const char* LibName, int major = -1, int minor = -1);
};