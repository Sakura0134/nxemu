#pragma once
#include "Textures\Texture.h"
#include <stdint.h>
#include <vector>

class CVideoMemory;

class OpenGLTICEntryTable
{
public:
    OpenGLTICEntryTable(CVideoMemory& VideoMemory);

    bool Read(uint32_t Index, TextureTICEntry & Entry);
    bool Synchornize(uint64_t GpuAddr, uint32_t Limit);

    uint32_t Limit() const { return m_CurrentLimit; }

private:
    OpenGLTICEntryTable(void);
    OpenGLTICEntryTable(const OpenGLTICEntryTable&);
    OpenGLTICEntryTable& operator=(const OpenGLTICEntryTable&);

    void Refresh(uint64_t GpuAddr, uint32_t Limit);

    CVideoMemory & m_VideoMemory;
    uint64_t m_CurrentGPUAddr;
    uint32_t m_CurrentLimit;
    std::vector<uint64_t> m_ReadEntries;
    std::vector<TextureTICEntry> m_Entries;
};