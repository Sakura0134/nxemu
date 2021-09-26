#pragma once
#include "Textures\Texture.h"
#include <stdint.h>
#include <vector>

class CVideoMemory;

class OpenGLTSCEntryTable
{
public:
    OpenGLTSCEntryTable(CVideoMemory & VideoMemory);

    bool Read(uint32_t index, TextureTSCEntry & Entry);
    bool Synchornize(uint64_t GpuAddr, uint32_t Limit);

    uint32_t Limit() const { return m_CurrentLimit; }

private:
    OpenGLTSCEntryTable(void);
    OpenGLTSCEntryTable(const OpenGLTSCEntryTable&);
    OpenGLTSCEntryTable& operator=(const OpenGLTSCEntryTable&);

    void Refresh(uint64_t GpuAddr, uint32_t Limit);
    bool IsDescriptorRead(uint32_t Index) const;
    void MarkDescriptorAsRead(uint32_t Index);

    CVideoMemory & m_VideoMemory;
    uint64_t m_CurrentGPUAddr;
    uint32_t m_CurrentLimit;
    std::vector<uint64_t> m_ReadEntries;
    std::vector<TextureTSCEntry> m_Entries;
};