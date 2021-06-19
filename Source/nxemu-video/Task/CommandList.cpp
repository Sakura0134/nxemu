#include "CommandList.h"
#include "VideoNotification.h"

CommandListTask::CommandListTask(ISwitchSystem & SwitchSystem, CVideo & Video, IRenderer & Renderer, const uint64_t * Commands, uint32_t NoOfCommands)
{
}

void CommandListTask::Execute(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}