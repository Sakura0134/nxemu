#pragma once
#include <stdint.h>

#ifndef EXPORT
#if defined(_WIN32)
#define EXPORT  extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C" __attribute__((visibility("default")))
#endif
#endif

#ifndef CALL
#if defined(_WIN32)
#define CALL __cdecl
#else
#define CALL
#endif
#endif

enum
{
    VIDEO_SPECS_VERSION = 0x0100
};

enum PLUGIN_TYPE
{
    PLUGIN_TYPE_VIDEO = 1,
};

typedef struct
{
    uint16_t Version;    // Should be set plugin spec version eg VIDEO_SPECS_VERSION
    uint16_t Type;       // Set to the plugin type, eg PLUGIN_TYPE_VIDEO
    char Name[200];      // Name of the DLL
} PLUGIN_INFO;

__interface IRenderWindow
{
    void * RenderSurface(void) const;
};

__interface ISwitchSystem
{
    uint8_t * GetCPUMemoryPointer(uint64_t CpuAddr);
    //const uint8_t * GetCPUMemoryPointer(uint64_t CpuAddr) const;*/
    bool ReadCPUMemory(uint64_t CpuAddr, void * DestBuffer, uint64_t Size) const;
    bool WriteCPUMemory(uint64_t CpuAddr, const void * SrcBuffer, uint64_t Size);
    void MarkRasterizerMemory(uint64_t CpuAddr, uint64_t size, bool cached);
    uint64_t TitleID(void);
};

/******************************************************************
  Function: GetPluginInfo
  Purpose:  This function allows the emulator to gather information
  about the dll by filling in the PluginInfo structure.
  input:    a pointer to a PLUGIN_INFO stucture that needs to be
  filled by the function. (see def above)
  output:   none
*******************************************************************/
EXPORT void CALL GetPluginInfo(PLUGIN_INFO * PluginInfo);

__interface IPluginNotification
{
    void DisplayError(const char* Message);
    void FatalError(const char* Message);
    void DisplayMessage(const char* Message);
    void DisplayMessageTime(int DisplayTime, const char* Message);
    void BreakPoint(const char* FileName, uint32_t LineNumber);
};

/******************************************************************
  Function: SetPluginNotification
  Purpose:  This function gives the plugin the ability to notify
            the emulator with messages.
  input:    a pointer to a IPluginNotification interface
  output:   none
*******************************************************************/
EXPORT void CALL SetPluginNotification(IPluginNotification & Notification);
