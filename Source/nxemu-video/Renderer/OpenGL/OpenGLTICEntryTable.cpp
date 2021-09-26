#include "OpenGLTICEntryTable.h"
#include "VideoMemoryManager.h"
#include "VideoNotification.h"

OpenGLTICEntryTable::OpenGLTICEntryTable(CVideoMemory & VideoMemory) :
    m_VideoMemory(VideoMemory), 
    m_CurrentGPUAddr(0),
    m_CurrentLimit(0)
{
}

bool OpenGLTICEntryTable::Read(uint32_t Index, TextureTICEntry & Entry)
{
    if (Index > m_CurrentLimit)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    uint64_t GpuAddr = m_CurrentGPUAddr + Index * sizeof(Entry);
    bool Result = false;
    m_VideoMemory.ReadBuffer(GpuAddr, &Entry, sizeof(Entry));
    if ((m_ReadEntries[Index / 64] & (1ULL << (Index % 64))) != 0)
    {
        Result = Entry != m_Entries[Index];
    }
    else 
    {
        m_ReadEntries[Index / 64] |= 1ULL << (Index % 64);
        Result = true;
    }
    if (Result)
    {
        m_Entries[Index] = Entry;
    }
    return Result;
}

bool OpenGLTICEntryTable::Synchornize(uint64_t GPUAddr, uint32_t Limit) 
{
    if (m_CurrentGPUAddr == GPUAddr && m_CurrentLimit == Limit) {
        return false;
    }
    Refresh(GPUAddr, Limit);
    return true;
}

void OpenGLTICEntryTable::Refresh(uint64_t GPUAddr, uint32_t Limit)
{
    m_CurrentGPUAddr = GPUAddr;
    m_CurrentLimit = Limit;

    uint32_t NumEntries = Limit + 1;

    m_ReadEntries.clear();
    m_ReadEntries.resize((NumEntries + 63U) / 64U, 0);
    m_Entries.resize(NumEntries);
}
