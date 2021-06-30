#pragma once
#include "GpuTask.h"
#include "Renderer/Renderer.h"

class CommandListEndTask :
    public CGpuTask 
{
public:
    void Execute(void);

    CommandListEndTask(IRenderer & Renderer);

private:
    CommandListEndTask();
    CommandListEndTask(const CommandListEndTask&);
    CommandListEndTask& operator=(const CommandListEndTask&);

    IRenderer & m_Renderer;
};