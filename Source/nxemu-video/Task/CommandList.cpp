#include "CommandList.h"
#include "Video.h"
#include "VideoNotification.h"
#include <algorithm>

CommandListTask::CommandListTask(CVideo & Video, IRenderer & Renderer, const uint64_t * Commands, uint32_t NoOfCommands) :
    m_Video(Video),
    m_Renderer(Renderer),
    m_CommandIndex(0),
    m_DmaMethod(BufferMethods_BindObject),
    m_DmaMethodCount(0),
    m_DmaSubchannel(0),
    m_DmaNonIncrementing(false),
    m_DmaIncrementOnce(false)
{
    m_CommandLists.resize(NoOfCommands);
    memcpy(m_CommandLists.data(), Commands, sizeof(Commands[0]) * NoOfCommands);
}

void CommandListTask::Execute(void)
{
    while (Step());
    m_Video.OnCommandListEnd();
}

bool CommandListTask::Step(void)
{
    if (m_CommandIndex >= m_CommandLists.size())
    {
        return false;
    }
    const CommandList & CmdList = m_CommandLists[m_CommandIndex++];
    Commands Cmds;
    Cmds.resize(CmdList.Size);
    m_Video.VideoMemory().ReadBuffer(CmdList.Addr(), Cmds.data(), CmdList.Size * sizeof(uint32_t));
    for (uint32_t i = 0, n = (uint32_t)Cmds.size(); i < n;)
    {
        const Command & Cmd = Cmds[i];

        if (m_DmaMethodCount)
        {
            if (m_DmaNonIncrementing)
            {
                uint32_t MaxWrite = std::min<uint32_t>(i + m_DmaMethodCount, n) - i;
                m_Video.CallMultiMethod((uint32_t)m_DmaMethod, m_DmaSubchannel, &Cmd.Value, MaxWrite, m_DmaMethodCount);
                m_DmaMethodCount -= MaxWrite;
                i += MaxWrite;
                continue;
            }
            else 
            {
                m_Video.CallMethod(m_DmaMethod, Cmd.Value, m_DmaSubchannel, m_DmaMethodCount);
            }
            if (!m_DmaNonIncrementing)
            {
                (uint32_t&)m_DmaMethod += 1;
            }
            if (m_DmaIncrementOnce)
            {
                m_DmaNonIncrementing = true;
            }
            m_DmaMethodCount--;
        }
        else
        {
            switch (Cmd.Mode) 
            {
            case SubmissionMode_Increasing:
                m_DmaMethod = Cmd.Method;
                m_DmaSubchannel = Cmd.SubChannel;
                m_DmaMethodCount = Cmd.ArgCount;
                m_DmaNonIncrementing = false;
                m_DmaIncrementOnce = false;
                break;
            case SubmissionMode_Inline:
                m_DmaMethod = Cmd.Method;
                m_DmaSubchannel = Cmd.SubChannel;
                m_Video.CallMethod(m_DmaMethod, Cmd.ArgCount, m_DmaSubchannel, m_DmaMethodCount);
                m_DmaNonIncrementing = true;
                m_DmaIncrementOnce = false;
                break;
            case SubmissionMode_IncreaseOnce:
                m_DmaMethod = Cmd.Method;
                m_DmaSubchannel = Cmd.SubChannel;
                m_DmaMethodCount = Cmd.ArgCount;
                m_DmaNonIncrementing = false;
                m_DmaIncrementOnce = true;
                break;
            case SubmissionMode_IncreasingOld:
                break;
            default:
                g_Notify->BreakPoint(__FILE__,__LINE__);
                break;
            }
        }
        i++;
    }
    return true;
}