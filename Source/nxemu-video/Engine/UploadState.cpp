#include "UploadState.h"
#include "VideoMemoryManager.h"
#include "VideoNotification.h"

CUploadState::CUploadState(CVideoMemory & MemoryManager, Registers & Regs) : 
    m_MemoryManager(MemoryManager),
    m_Regs(Regs), 
    m_WriteOffset(0),
    m_CopySize(0),
    m_Linear(false)
{
}

CUploadState::~CUploadState()
{
}

void CUploadState::ProcessExec(bool Linear)
{
    m_WriteOffset = 0;
    m_CopySize = m_Regs.LineLengthIn * m_Regs.LineCount;
    m_InnerBuffer.resize(m_CopySize);
    m_Linear = Linear;
}

void CUploadState::ProcessData(uint32_t Data, bool Last)
{
    uint32_t CopySize = m_CopySize - m_WriteOffset;
    if (CopySize > 4)
    {
        CopySize = 4;
    }
    memcpy(&m_InnerBuffer[m_WriteOffset], &Data, CopySize);
    m_WriteOffset += CopySize;
    if (Last)
    {
        uint64_t Address = m_Regs.Dest.Address();
        if (m_Linear)
        {
            m_MemoryManager.WriteBuffer(Address, m_InnerBuffer.data(), m_CopySize, true);
        }
        else
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
}
