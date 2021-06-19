#pragma once
#include "GpuTask.h"
#include "Renderer/Renderer.h"

__interface ISwitchSystem;
class CVideo;

class CommandListTask :
    public CGpuTask 
{
public:
    CommandListTask(ISwitchSystem & SwitchSystem, CVideo & Video, IRenderer & Renderer, const uint64_t* Commands, uint32_t NoOfCommands);
    void Execute(void);

private:
    CommandListTask();
    CommandListTask(const CommandListTask&);
    CommandListTask& operator=(const CommandListTask&);
};