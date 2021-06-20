#include "CommandList.h"
#include "Video.h"
#include "VideoNotification.h"

CommandListTask::CommandListTask(ISwitchSystem & /*SwitchSystem*/, CVideo & Video, IRenderer & /*Renderer*/, const uint64_t * Commands, uint32_t NoOfCommands) :
    m_Video(Video),
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
    for (size_t i = 0, n = Cmds.size(); i < n;)
    {
        const Command & Cmd = Cmds[i];

        if (m_DmaMethodCount)
        {
            if (m_DmaNonIncrementing)
            {
                g_Notify->BreakPoint(__FILE__,__LINE__);
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