#include "EmulatorWindow.h"
#include <algorithm>

EmulatorWindow::EmulatorWindow(IRenderWindow & RenderWindow) :
    m_RenderSurface(RenderWindow.RenderSurface()),
    m_Width(640),
    m_Height(480),
    m_Screen(0, 0, 0, 0)
{
    enum
    {
        ScreenUndockedWidth = 1280,
        ScreenUndockedHeight = 720,
    };

    float WindowAspectRatio = (float)m_Height / (float)m_Width;
    float EmulationAspectRatio = (float)ScreenUndockedHeight / (float)ScreenUndockedWidth;

    WindowScreen ScreenWindowArea(0, 0, m_Width, m_Height);
    float Scale = std::min((float)m_Width, ((float)m_Height) / EmulationAspectRatio);
    m_Screen = WindowScreen(0, 0, (uint32_t)Scale, (uint32_t)(Scale * EmulationAspectRatio));
    if (WindowAspectRatio < EmulationAspectRatio)
    {
        uint32_t X = (ScreenWindowArea.Width() - m_Screen.Width()) / 2;
        m_Screen = WindowScreen(m_Screen.Left() + X, m_Screen.Top(), m_Screen.Right() + X, m_Screen.Bottom());
    }
    else
    {
        uint32_t Y = (m_Height - m_Screen.Height()) / 2;
        m_Screen = WindowScreen(m_Screen.Left(), m_Screen.Top() + Y, m_Screen.Right(), m_Screen.Bottom() + Y);
    }
}
