#pragma once
#include "GpuTask.h"
#include "GpuTypes.h"
#include "Renderer/Renderer.h"
#include <stdint.h>
#include <vector>

__interface ISwitchSystem;
class CVideo;

class CommandListTask :
    public CGpuTask 
{
    enum SubmissionMode : unsigned
    {
        SubmissionMode_IncreasingOld = 0,
        SubmissionMode_Increasing = 1,
        SubmissionMode_NonIncreasingOld = 2,
        SubmissionMode_NonIncreasing = 3,
        SubmissionMode_Inline = 4,
        SubmissionMode_IncreaseOnce = 5
    };

#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
    union CommandList
    {
        uint64_t Value;
        struct
        {
            unsigned AddrLo : 32;
            unsigned AddrHi : 8;
            unsigned : 1;
            unsigned IsNonMain : 1;
            unsigned Size : 21;
        };
        uint64_t Addr(void) const { return (((uint64_t)AddrHi) << 32) | AddrLo; }
    };
    union Command 
    {
        uint32_t Value;
        struct 
        {
            BufferMethods Method : 13;
            unsigned SubChannel : 3;
            unsigned ArgCount : 13;
            SubmissionMode Mode : 3;
        };
    };
#pragma warning(pop)

    typedef std::vector<CommandList> CommandLists;
    typedef std::vector<Command> Commands;

public:
    CommandListTask(CVideo & Video, IRenderer & Renderer, const uint64_t* Commands, uint32_t NoOfCommands);
    void Execute(void);

private:
    CommandListTask();
    CommandListTask(const CommandListTask&);
    CommandListTask& operator=(const CommandListTask&);

    bool Step(void);

    CVideo & m_Video;
    IRenderer & m_Renderer; 
    CommandLists m_CommandLists;
    uint32_t m_CommandIndex;
    BufferMethods m_DmaMethod;
    uint32_t m_DmaMethodCount;
    uint32_t m_DmaSubchannel;
    bool m_DmaNonIncrementing;
    bool m_DmaIncrementOnce;
};