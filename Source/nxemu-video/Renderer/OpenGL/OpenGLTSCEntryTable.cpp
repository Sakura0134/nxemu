#include "OpenGLTSCEntryTable.h"
#include "VideoMemoryManager.h"
#include "VideoNotification.h"

OpenGLTSCEntryTable::OpenGLTSCEntryTable(CVideoMemory & VideoMemory) :
    m_VideoMemory(VideoMemory), 
    m_CurrentGPUAddr(0),
    m_CurrentLimit(0)
{
}

bool OpenGLTSCEntryTable::Read(uint32_t Index, TextureTSCEntry & Entry)
{
    if (Index > m_CurrentLimit)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    uint64_t GpuAddr = m_CurrentGPUAddr + (Index * sizeof(Entry));
    bool Result = false;
    m_VideoMemory.ReadBuffer(GpuAddr, &Entry, sizeof(Entry));
    if (IsDescriptorRead(Index)) 
    {
        Result = Entry != m_Entries[Index];
    }
    else 
    {
        MarkDescriptorAsRead(Index);
        Result = true;
    }
    if (Result)
    {
        m_Entries[Index] = Entry;
    }
    return Result;
}

bool OpenGLTSCEntryTable::Synchornize(uint64_t GPUAddr, uint32_t Limit) 
{
    if (m_CurrentGPUAddr == GPUAddr && m_CurrentLimit == Limit) 
    {
        return false;
    }
    Refresh(GPUAddr, Limit);
    return true;
}

void OpenGLTSCEntryTable::Refresh(uint64_t GPUAddr, uint32_t Limit)
{
    m_CurrentGPUAddr = GPUAddr;
    m_CurrentLimit = Limit;

    uint32_t NumEntries = Limit + 1;

    m_ReadEntries.clear();
    m_ReadEntries.resize((NumEntries + 63U) / 64U, 0);
    m_Entries.resize(NumEntries);
}

bool OpenGLTSCEntryTable::IsDescriptorRead(uint32_t Index) const 
{
    return (m_ReadEntries[Index / 64] & (1ULL << (Index % 64))) != 0;
}

void OpenGLTSCEntryTable::MarkDescriptorAsRead(uint32_t Index) 
{
    m_ReadEntries[Index / 64] |= 1ULL << (Index % 64);
}
