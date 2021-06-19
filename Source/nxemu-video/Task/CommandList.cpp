#include "CommandList.h"
#include "Video.h"
#include "VideoNotification.h"

CommandListTask::CommandListTask(ISwitchSystem & SwitchSystem, CVideo & Video, IRenderer & Renderer, const uint64_t * Commands, uint32_t NoOfCommands) :
    m_Video(Video),
    m_CommandIndex(0)
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

        switch (Cmd.Mode) 
        {
        default:
            g_Notify->BreakPoint(__FILE__,__LINE__);
            break;
        }
        i++;
    }
    return true;
}