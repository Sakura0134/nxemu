#include "CommandListEnd.h"
#include "Video.h"
#include "VideoNotification.h"

CommandListEndTask::CommandListEndTask(IRenderer & Renderer) :
    m_Renderer(Renderer)
{
}

void CommandListEndTask::Execute(void)
{
    m_Renderer.ReleaseFences();
}