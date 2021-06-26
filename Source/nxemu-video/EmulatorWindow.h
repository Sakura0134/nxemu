#pragma once
#include <nxemu-plugin-spec\Video.h>

class EmulatorWindow 
{
public:
    EmulatorWindow(IRenderWindow& RenderWindow);
    void * RenderSurface(void) const { return m_RenderWindow.RenderSurface(); }

private:
    EmulatorWindow();
    EmulatorWindow(const EmulatorWindow&);
    EmulatorWindow& operator=(const EmulatorWindow&);

    IRenderWindow & m_RenderWindow;
};