#pragma once
#include "Renderer\Renderer.h"
#include "Task\GpuTask.h"
#include <nxemu-plugin-spec\Video.h>
#include <Common\Thread.h>
#include <Common\CriticalSection.h>
#include <Common\SyncEvent.h>
#include <memory>
#include <list>

class CVideo;

class CGpuThread :
    private CThread
{
    typedef std::shared_ptr<CGpuTask> GpuTask;
    typedef std::list<GpuTask> Tasklist;

public:
    CGpuThread(ISwitchSystem & SwitchSystem, CVideo & Video);
    ~CGpuThread();

    bool StartThread(void);
    void PushCommands(const uint64_t * Entries, uint32_t NoOfEntries);

private:
    CGpuThread();
    CGpuThread(const CGpuThread&);
    CGpuThread& operator=(const CGpuThread&);

    void GpuThread(void);
    static uint32_t stGpuThread(void* _this) { ((CGpuThread*)_this)->GpuThread(); return 0; }

    void PushCommand(GpuTask && Task);

    std::unique_ptr<IRenderer> m_Renderer;
    CriticalSection m_cs;
    Tasklist m_Tasks;
    ISwitchSystem & m_SwitchSystem;
    CVideo & m_Video;
    bool m_RenderInit;
    bool m_Running;
    SyncEvent m_TaskEvent;
};
