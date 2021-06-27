#pragma once
#include <vector>
#include <stdint.h>

class CVideoMemory;

class CUploadState
{
public:
    struct Registers 
    {
        uint32_t LineLengthIn;
        uint32_t LineCount;

        struct 
        {
            uint32_t AddrHigh;
            uint32_t AddrLow;
            uint32_t Pitch;
            uint32_t Block;
            uint32_t Width;
            uint32_t Height;
            uint32_t Depth;
            uint32_t Z;
            uint32_t X;
            uint32_t Y;

            uint64_t Address() const 
            {
                return (((uint64_t)AddrHigh) << 32) | AddrLow;
            }
        } Dest;
    };

    CUploadState(CVideoMemory & MemoryManager, Registers & Regs);
    ~CUploadState();

    void ProcessExec(bool Linear);
    void ProcessData(uint32_t Data, bool Last);

private:
    CUploadState();
    CUploadState(const CUploadState&);
    CUploadState& operator=(const CUploadState&);

    CVideoMemory & m_MemoryManager;
    Registers & m_Regs;
    std::vector<uint8_t> m_InnerBuffer;
    uint32_t m_WriteOffset;
    uint32_t m_CopySize;
    bool m_Linear;
};