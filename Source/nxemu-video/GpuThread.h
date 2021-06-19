#pragma once
#include "Renderer\Renderer.h"
#include <nxemu-plugin-spec\Video.h>
#include <Common\Thread.h>
#include <memory>

class CVideo;

class CGpuThread :
    private CThread
{
public:
    CGpuThread(ISwitchSystem & SwitchSystem, CVideo & Video);
    ~CGpuThread();

    bool StartThread(void);
private:
    CGpuThread();
    CGpuThread(const CGpuThread&);
    CGpuThread& operator=(const CGpuThread&);

    void GpuThread(void);
    static uint32_t stGpuThread(void* _this) { ((CGpuThread*)_this)->GpuThread(); return 0; }

    std::unique_ptr<IRenderer> m_Renderer;
    ISwitchSystem & m_SwitchSystem;
    CVideo & m_Video;
    bool m_RenderInit;
};
