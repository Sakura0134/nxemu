#pragma once

template <class T>
class CRectangle 
{
public:
    CRectangle(T Left, T Top, T Right, T Bottom) : 
        m_Left(Left),
        m_Top(Top), 
        m_Right(Right), 
        m_Bottom(Bottom)
    {
    }

    inline T Left() const { return m_Left; }
    inline T Top() const { return m_Top; }
    inline T Right() const { return m_Right; }
    inline T Bottom() const { return m_Bottom; }

    T Width() const { return m_Right - m_Left;}
    T Height() const { return m_Bottom - m_Top;}

private:
    T m_Left;
    T m_Top;
    T m_Right;
    T m_Bottom;
};

