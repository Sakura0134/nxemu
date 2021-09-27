#pragma once
#include <nxemu-plugin-spec\Video.h>
#include "util/Rectangle.h"

typedef CRectangle<uint32_t> WindowScreen;

class EmulatorWindow 
{
public:
    EmulatorWindow(IRenderWindow& RenderWindow);

    void * RenderSurface(void) const { return m_RenderSurface; }
    uint32_t Width(void) const { return m_Width; }
    uint32_t Height(void) const { return m_Height; }
    const WindowScreen & Screen(void) const { return m_Screen; }

private:
    EmulatorWindow();
    EmulatorWindow(const EmulatorWindow&);
    EmulatorWindow& operator=(const EmulatorWindow&);

    void * m_RenderSurface;
    uint32_t m_Width;
    uint32_t m_Height;
    WindowScreen m_Screen;
};